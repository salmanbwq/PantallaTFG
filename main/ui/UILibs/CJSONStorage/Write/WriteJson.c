//
// Created by salman on 15/02/25.
//

#include "WriteJson.h"

#include "cJSON/cJSON.h"
#include <esp_log.h>
#include <string.h>
#include <esp_spiffs.h>
#include <sys/unistd.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>

static const char *TAG = "Write JSON";


static void addDevicesListJson(const cJSON *json, const char *FILE_PATH) {
    FILE *file = fopen(FILE_PATH, "w");
    if (!file) {
        ESP_LOGE(TAG, "No se pudo abrir el archivo JSON para escribir.");
        return;
    }

    char *json_str = cJSON_PrintUnformatted(json);
    if (!json_str) {
        ESP_LOGE(TAG, "Error al convertir cJSON a string.");
        fclose(file);
        return;
    }

    fwrite(json_str, 1, strlen(json_str), file);
    fclose(file);
    free(json_str);

    ESP_LOGI(TAG, "Dispositivos RF guardados en JSON correctamente.");
}


void updateDevicesListJson(const cJSON *newJson, const char *FILE_PATH) {
    // Leer JSON existente
    cJSON *existingJson = readDevices(FILE_PATH);
    if (!existingJson || !cJSON_IsArray(existingJson)) {
        ESP_LOGW(TAG, "No se pudo abrir el archivo JSON o está corrupto, se creará uno nuevo.");
        existingJson = cJSON_CreateArray();
    }

    // Validar que el JSON a agregar sea válido
    if (!newJson || !cJSON_IsObject(newJson)) {
        ESP_LOGE(TAG, "El nuevo JSON no es válido.");
        cJSON_Delete(existingJson);
        return;
    }

    // Obtener nombre del nuevo dispositivo con verificación
    cJSON *nameObj = cJSON_GetObjectItem(newJson, "name");
    if (!nameObj || !cJSON_IsString(nameObj)) {
        ESP_LOGE(TAG, "El nuevo dispositivo no tiene nombre válido.");
        cJSON_Delete(existingJson);
        return;
    }
    const char *newDeviceName = nameObj->valuestring;

    // Buscar si el dispositivo ya existe
    bool found = false;
    int index = 0;

    cJSON *device = NULL;
    cJSON_ArrayForEach(device, existingJson) {
        cJSON *name = cJSON_GetObjectItem(device, "name");
        if (name && strcmp(name->valuestring, newDeviceName) == 0) {
            // Dispositivo encontrado, eliminarlo antes de reemplazarlo
            cJSON_Delete(cJSON_DetachItemFromArray(existingJson, index));
            cJSON_InsertItemInArray(existingJson, index, cJSON_Duplicate(newJson, 1));
            found = true;
            ESP_LOGI(TAG, "Dispositivo '%s' actualizado.", newDeviceName);
            break;
        }
        index++;
    }

    // Si no se encontró, agregarlo
    if (!found) {
        ESP_LOGI(TAG, "Dispositivo '%s' no encontrado. Agregando nuevo.", newDeviceName);
        cJSON_AddItemToArray(existingJson, cJSON_Duplicate(newJson, 1));
    }

    // Guardar JSON actualizado
    FILE *file = fopen(FILE_PATH, "w");
    if (!file) {
        ESP_LOGE(TAG, "No se pudo abrir el archivo JSON para escribir.");
        cJSON_Delete(existingJson);
        return;
    }

    // Usar `cJSON_PrintUnformatted()` para evitar truncamientos
    char *json_str = cJSON_PrintUnformatted(existingJson);
    if (!json_str) {
        ESP_LOGE(TAG, "Error al convertir cJSON a string.");
        fclose(file);
        cJSON_Delete(existingJson);
        return;
    }

    ESP_LOGI(TAG, "JSON generado antes de escribir: %s", json_str);
    fwrite(json_str, 1, strlen(json_str), file);
    fsync(fileno(file)); // Asegurar escritura en el sistema de archivos
    fclose(file);
    free(json_str);
    cJSON_Delete(existingJson); // Liberar memoria

    ESP_LOGI(TAG, "Dispositivos guardados en JSON correctamente.");
}


static bool deviceExistsInJson(const cJSON *json, const char *name) {
    int count = cJSON_GetArraySize(json);
    for (int i = 0; i < count; i++) {
        cJSON *device = cJSON_GetArrayItem(json, i);
        const char *existingName = cJSON_GetObjectItem(device, "name")->valuestring;

        if (strcmp(existingName, name) == 0) {
            return true; // El dispositivo ya existe
        }
    }
    return false; // No hay duplicados
}

void AddDevicesJson(cJSON *new_device, const char *FILE_PATH) {
    cJSON *json = readDevices(FILE_PATH);

    if (!json) {
        ESP_LOGE(TAG, "No se pudo encontrar el archivo JSON");
        return;
    }

    if (!new_device) {
        ESP_LOGE(TAG, "No se recibio ningun device");
        return;
    }


    char *name = cJSON_GetObjectItem(new_device, "name")->valuestring;

    // Verificar si el dispositivo ya existe
    if (deviceExistsInJson(json, name)) {
        ESP_LOGW(TAG, "El dispositivo con nombre '%s' ya existe. No se agregará.", name);
        cJSON_Delete(json);
        return;
    }

    // Obtener un nuevo ID
    int new_id = cJSON_GetArraySize(json) + 1;

    // Crear nuevo objeto JSON
    cJSON_AddNumberToObject(new_device, "id", new_id);

    // Agregar al JSON
    cJSON_AddItemToArray(json, new_device);

    // Guardar cambios
    addDevicesListJson(json, FILE_PATH);
    // cJSON_Delete(json);

    ESP_LOGI(TAG, "Nuevo dispositivo agregado: ID %d, Nombre: %s", new_id, name);
}
