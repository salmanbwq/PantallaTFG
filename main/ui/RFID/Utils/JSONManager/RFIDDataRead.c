//
// Created by salman on 16/02/25.
//

#include "RFIDDataRead.h"

#include <esp_log.h>
#include <string.h>
#include <cJSON/cJSON.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/CJSONStorage/Write/WriteJson.h>
#define FILE_PATH "/spiffs/rfidDevices.json"

static const char *TAG = "RFIDDataRead";

char *getCommandByName(char *dispName, const char *commandName) {
    cJSON *json = getJsonByName(dispName, FILE_PATH);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist: %s", dispName);
        cJSON_Delete(json);
        return NULL;
    }

    ESP_LOGI(TAG, "Command name: %s", commandName);

    cJSON *command_array = cJSON_GetObjectItem(json, "command");
    if (!cJSON_IsArray(command_array)) {
        ESP_LOGE(TAG, "El campo 'command' no es un array.");
        cJSON_Delete(json);
        return NULL;
    }


    int count = cJSON_GetArraySize(command_array);

    for (int i = 0; i < count; i++) {
        cJSON *command = cJSON_GetArrayItem(command_array, i);
        cJSON *cmd_name = cJSON_GetObjectItem(command, "name");

        if (cJSON_IsString(cmd_name) && strcmp(cmd_name->valuestring, commandName) == 0) {
            ESP_LOGI(TAG, "Command name: %s", cmd_name->valuestring);
            ESP_LOGI(TAG, "Command content: %s", cJSON_GetObjectItem(command, "content")->valuestring);
            return cJSON_GetObjectItem(command, "content")->valuestring;
        }
    }
    return NULL;
}
