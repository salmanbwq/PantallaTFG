//
// Created by salman on 15/02/25.
//

#include "JSONManager.h"

#include <esp_log.h>
#include <esp_spiffs.h>
#include <cJSON/cJSON.h>

#include "Write/WriteJson.h"


void AddDevices(cJSON *new_device, const char *FILE_PATH) {
    AddDevicesJson(new_device, FILE_PATH);
}

void init_spiffs() {
    ESP_LOGI("SPIFFS", "Montando SPIFFS...");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true // Formatear si falla el montaje
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE("SPIFFS", "Error al montar SPIFFS (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI("SPIFFS", "SPIFFS montado correctamente.");
    }
}
