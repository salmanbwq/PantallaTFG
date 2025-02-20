//
// Created by salman on 15/02/25.
//

#include "WriteJson.h"

#include "cJSON/cJSON.h"
#include <esp_log.h>
#include <string.h>
#include <esp_spiffs.h>

static const char *TAG = "Write JSON";

static cJSON *readDevices(const char *FILE_PATH) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        ESP_LOGW(TAG, "No se encontró el archivo JSON, creando uno nuevo.");
        return cJSON_CreateArray(); // Devuelve un JSON vacío si no hay archivo
    }

    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        ESP_LOGE(TAG, "Error al parsear JSON.");
        return cJSON_CreateArray();
    }

    return json;
}

static void addDevicesListJson(const cJSON *json, const char *FILE_PATH) {
    FILE *file = fopen(FILE_PATH, "w");
    if (!file) {
        ESP_LOGE(TAG, "No se pudo abrir el archivo JSON para escribir.");
        return;
    }

    char *json_str = cJSON_Print(json);
    fwrite(json_str, 1, strlen(json_str), file);
    fclose(file);
    free(json_str);

    ESP_LOGI(TAG, "Dispositivos RF guardados en JSON.");
}

void updateDevicesListJson(const cJSON *newJson, const char *FILE_PATH) {
    // Leer el JSON existente desde el archivo
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        ESP_LOGW(TAG, "No se pudo abrir el archivo JSON para leer, se creará uno nuevo.");
    }

    cJSON *existingJson = NULL;
    if (file) {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        char *fileBuffer = (char *) malloc(fileSize + 1);
        if (fileBuffer) {
            fread(fileBuffer, 1, fileSize, file);
            fileBuffer[fileSize] = '\0'; // Asegurar terminación de cadena
            existingJson = cJSON_Parse(fileBuffer);
            free(fileBuffer);
        }
        fclose(file);
    }

    // Si no hay JSON existente, creamos un nuevo array
    if (!existingJson) {
        existingJson = cJSON_CreateArray();
    }

    // Buscar si el dispositivo ya existe en el JSON
    const char *newDeviceName = cJSON_GetObjectItem(newJson, "name")->valuestring;
    cJSON *device = NULL;
    cJSON_ArrayForEach(device, existingJson) {
        cJSON *name = cJSON_GetObjectItem(device, "name");
        if (name && strcmp(name->valuestring, newDeviceName) == 0) {
            // Dispositivo encontrado, lo reemplazamos con `newJson`
            cJSON_ReplaceItemInArray(existingJson, cJSON_GetArraySize(existingJson) - 1, cJSON_Duplicate(newJson, 1));
            break;
        }
    }

    // Si no se encontró, agregar el nuevo dispositivo
    if (!device) {
        cJSON_AddItemToArray(existingJson, cJSON_Duplicate(newJson, 1));
    }

    // Escribir el JSON actualizado al archivo
    file = fopen(FILE_PATH, "w");
    if (!file) {
        ESP_LOGE(TAG, "No se pudo abrir el archivo JSON para escribir.");
        cJSON_Delete(existingJson);
        return;
    }

    char *json_str = cJSON_Print(existingJson);
    fwrite(json_str, 1, strlen(json_str), file);
    fclose(file);
    free(json_str);
    cJSON_Delete(existingJson); // Liberar memoria

    ESP_LOGI(TAG, "Dispositivos RF guardados en JSON correctamente.");
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

cJSON *getJsonByName(const char *name, const char *FILE_PATH) {
    cJSON *json = readDevices(FILE_PATH);
    int count = cJSON_GetArraySize(json);
    for (int i = 0; i < count; i++) {
        cJSON *device = cJSON_GetArrayItem(json, i);
        const char *existingName = cJSON_GetObjectItem(device, "name")->valuestring;
        if (strcmp(existingName, name) == 0) {
            return device;
        }
    }
    ESP_LOGE(TAG, "Json for %s not found", name);
    return NULL;
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
    cJSON_Delete(json);

    ESP_LOGI(TAG, "Nuevo dispositivo agregado: ID %d, Nombre: %s", new_id, name);
}
