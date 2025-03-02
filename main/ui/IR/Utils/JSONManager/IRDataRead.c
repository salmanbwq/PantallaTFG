//
// Created by salman on 21/02/25.
//

#include "IRDataRead.h"
#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <misc/lv_types.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
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
        return NULL;
    }

    cJSON *typeItem = cJSON_GetObjectItemCaseSensitive(json, "type");
    if (!cJSON_IsString(typeItem) || (typeItem->valuestring == NULL)) {
        ESP_LOGE(TAG, "Not found type for: %s", name);
        cJSON_Delete(json);
        return NULL;
    }

    char *deviceType = strdup(typeItem->valuestring); // Copia segura

    cJSON_Delete(json); // Liberar json después de usarlo
    return deviceType;
}

char *getIRCommand(char *deviceName, const char *commandName) {
    cJSON *json = getDeviceFromJson(deviceName);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist for device %s", deviceName);
        return NULL;
    }

    cJSON *commandArray = cJSON_GetObjectItemCaseSensitive(json, "command");
    if (!cJSON_IsArray(commandArray)) {
        ESP_LOGE(TAG, "Invalid command format for: %s", deviceName);
        cJSON_Delete(json);
        return NULL;
    }

    char *commandContent = NULL;

    for (int i = 0; i < cJSON_GetArraySize(commandArray); i++) {
        cJSON *item = cJSON_GetArrayItem(commandArray, i);
        cJSON *nameItem = cJSON_GetObjectItem(item, "name");
        cJSON *contentItem = cJSON_GetObjectItem(item, "content");

        if (cJSON_IsString(nameItem) && cJSON_IsString(contentItem) &&
            strcmp(nameItem->valuestring, commandName) == 0) {
            commandContent = strdup(contentItem->valuestring);
            break; // Terminar búsqueda cuando se encuentre el comando
        }
    }

    cJSON_Delete(json); // Liberar json después de usarlo

    if (commandContent == NULL || strlen(commandContent) == 0) {
        ESP_LOGE(TAG, "Not found content for: %s", deviceName);
        return NULL;
    } else {
        ESP_LOGI(TAG, "Command stored is %s", commandContent);
    }

    return commandContent;
}
