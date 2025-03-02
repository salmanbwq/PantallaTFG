//
// Created by salman on 15/02/25.
//
#include "ReadJson.h"
#include <stdlib.h>
#include <string.h>
#include "lvgl.h"
#include "cJSON/cJSON.h"
#include <esp_log.h>
#include <stdio.h>

static char *TAG = "ReadJson";

cJSON *readDevices(const char *FILE_PATH) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        ESP_LOGW(TAG, "No se encontró el archivo JSON, creando uno nuevo.");
        return cJSON_CreateArray();
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    if (fileSize <= 0) {
        ESP_LOGE(TAG, "El archivo JSON está vacío o corrupto.");
        fclose(file);
        return cJSON_CreateArray();
    }

    char *buffer = (char *) malloc(fileSize + 1);
    if (!buffer) {
        ESP_LOGE(TAG, "Error al asignar memoria para JSON.");
        fclose(file);
        return cJSON_CreateArray();
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer); // Liberar buffer después de parsear

    if (!json) {
        ESP_LOGE(TAG, "Error al parsear JSON.");
        return cJSON_CreateArray();
    }

    return json; // ¡Ojo! Quien llame a esta función debe liberar `json` con `cJSON_Delete()`
}

void populateDropdownNames(lv_obj_t *dropdw, const char *FILE_PATH) {
    lv_dropdown_clear_options(dropdw);

    cJSON *json = readDevices(FILE_PATH);
    if (!json || !cJSON_IsArray(json)) {
        ESP_LOGE(TAG, "Error al leer JSON.");
        cJSON_Delete(json);
        return;
    }

    int count = cJSON_GetArraySize(json);
    if (count == 0) {
        ESP_LOGW(TAG, "No hay dispositivos en el JSON.");
        cJSON_Delete(json);
        return;
    }

    for (int i = 0; i < count; i++) {
        cJSON *device = cJSON_GetArrayItem(json, i);
        cJSON *nameItem = cJSON_GetObjectItem(device, "name");

        if (!nameItem || !cJSON_IsString(nameItem)) {
            ESP_LOGW(TAG, "Dispositivo sin nombre válido, omitiendo.");
            continue;
        }

        const char *device_name = nameItem->valuestring;
        ESP_LOGI(TAG, "Agregando dispositivo: %s", device_name);
        lv_dropdown_add_option(dropdw, device_name, i);
    }

    cJSON_Delete(json); // Liberar memoria correctamente
    ESP_LOGI(TAG, "Dropdown actualizado con %d dispositivos.", count);
}

cJSON *getJsonByName(const char *name, const char *FILE_PATH) {
    cJSON *json = readDevices(FILE_PATH);
    if (!json || !cJSON_IsArray(json)) {
        ESP_LOGE(TAG, "Error al leer JSON.");
        cJSON_Delete(json);
        return NULL;
    }

    cJSON *deviceCopy = NULL;

    for (int i = 0; i < cJSON_GetArraySize(json); i++) {
        cJSON *device = cJSON_GetArrayItem(json, i);
        cJSON *nameItem = cJSON_GetObjectItem(device, "name");

        if (cJSON_IsString(nameItem) && strcmp(nameItem->valuestring, name) == 0) {

            deviceCopy = cJSON_Duplicate(device, 1); // Clonar el objeto antes de liberar el JSON
            break;
        }
    }

    cJSON_Delete(json); // Liberar el JSON original

    if (!deviceCopy) {
        ESP_LOGE(TAG, "Json for %s not found", name);
    }

    return deviceCopy; // Retorna el JSON del dispositivo encontrado (debe liberarse con `cJSON_Delete()`)
}
