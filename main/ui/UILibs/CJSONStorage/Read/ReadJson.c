//
// Created by salman on 15/02/25.
//

#include "ReadJson.h"
#include <stdlib.h>
#include <string.h>
#include "lvgl.h"
#include "cJSON/cJSON.h"
#include "esp_spiffs.h"
#include <esp_log.h>
#include <stdio.h>
#include <ui/UILibs/CJSONStorage/Write/WriteJson.h>
#define MAX_DROPDOWN_ITEMS 10
static char *TAG = "ReadJson";

void populateDropdownNames(lv_obj_t *dropdw, const char *FILE_PATH) {
    // Limpiar dropdown antes de agregar nuevos elementos
    lv_dropdown_clear_options(dropdw);

    // Abrir el archivo JSON
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        ESP_LOGE(TAG, "No se pudo abrir el archivo JSON");
        return;
    }

    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    // Parsear el JSON
    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        ESP_LOGE(TAG, "Error al parsear JSON.");
        return;
    }

    int count = cJSON_GetArraySize(json);
    if (count == 0) {
        ESP_LOGW(TAG, "No hay dispositivos en el JSON.");
        cJSON_Delete(json);
        return;
    }

    for (int i = 0; i < count && i < MAX_DROPDOWN_ITEMS; i++) {
        cJSON *device = cJSON_GetArrayItem(json, i);
        const char *device_name = cJSON_GetObjectItem(device, "name")->valuestring;
        ESP_LOGI(TAG, "Device name: %s", device_name);

        lv_dropdown_add_option(dropdw, device_name, i);
    }

    cJSON_Delete(json);
    ESP_LOGI(TAG, "Dropdown actualizado con %d dispositivos.", count);
}
