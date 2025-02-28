//
// Created by salman on 21/02/25.
//

#include "IRDataRead.h"
#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <misc/lv_types.h>
#include <ui/UILibs/CJSONStorage/Write/WriteJson.h>
#define FILE_PATH "/spiffs/irDevices.json"

static const char *TAG = "IRDataRead";

static cJSON *getDeviceFromJson(const char *deviceName) {
    cJSON *json = getJsonByName(deviceName, FILE_PATH);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist: %s", deviceName);
        cJSON_Delete(json);
        return NULL;
    }
    return json;
}

char *getIRDeviceType(char *name) {
    cJSON *json = getDeviceFromJson(name);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist: %s", name);
        cJSON_Delete(json);
        return NULL;
    }

    char *deviceType = malloc(20 * sizeof(char));

    strcpy(deviceType, cJSON_GetObjectItemCaseSensitive(json, "type")->valuestring);
    if (strlen(deviceType) == 0) {
        ESP_LOGE(TAG, "Not found type for: %s", name);
        cJSON_Delete(json);
        return NULL;
    }
    return deviceType;
}

char *getIRCommand(char *deviceName, const char *commandName) {
    cJSON *json = getDeviceFromJson(deviceName);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist for device %s", deviceName);
        cJSON_Delete(json);
        return NULL;
    }

    char *commandContent = malloc(50 * sizeof(char));

    cJSON *command = cJSON_GetObjectItemCaseSensitive(json, "command");

    for (int i = 0; i < cJSON_GetArraySize(command); i++) {
        cJSON *item = cJSON_GetArrayItem(command, i);
        if (strcmp(cJSON_GetObjectItem(item, "name")->valuestring, commandName) == 0) {
            strcpy(commandContent, cJSON_GetObjectItemCaseSensitive(item, "content")->valuestring);
        }
    }


    if (strlen(commandContent) == 0) {
        ESP_LOGE(TAG, "Not found content for: %s", deviceName);
        cJSON_Delete(json);
        return NULL;
    }
    ESP_LOGI(TAG, "Command stored is %s", commandContent);
    return commandContent;
}
