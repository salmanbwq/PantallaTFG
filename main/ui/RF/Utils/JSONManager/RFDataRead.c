//
// Created by salman on 11/02/25.
//

#include "RFDataRead.h"

#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <misc/lv_types.h>
#include <ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.h>
#include <ui/UILibs/CJSONStorage/Write/WriteJson.h>

#define FILE_PATH "/spiffs/rf_devices.json"

#define MAX_DROPDOWN_ITEMS 10
static const char *TAG = "RFDataRead";

static cJSON *getDeviceFromJson(const char *deviceName) {
    cJSON *json = getJsonByName(deviceName, FILE_PATH);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist: %s", deviceName);
        cJSON_Delete(json);
        return NULL;
    }
    return json;
}

char *getDeviceType(char *name) {
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

char *getDeviceRF(char *deviceName) {
    cJSON *json = getDeviceFromJson(deviceName);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist: %s", deviceName);
        cJSON_Delete(json);
        return NULL;
    }

    char *deviceType = malloc(20 * sizeof(char));

    strcpy(deviceType, cJSON_GetObjectItemCaseSensitive(json, "freq")->valuestring);
    if (strlen(deviceType) == 0) {
        ESP_LOGE(TAG, "Not found type for: %s", deviceName);
        cJSON_Delete(json);
        return NULL;
    }
    return deviceType;
}

static char *getGarageCommand(const char *name, const char *commanName, cJSON *json, cJSON *commands) {
    if (commands == NULL) {
        ESP_LOGE(TAG, "File does not exist: %s", name);
        cJSON_Delete(json);
        return NULL;
    }
    cJSON *command;
    if (strcmp(commanName, "Open") == 0) {
        command = cJSON_GetArrayItem(commands, 0);
    } else if (strcmp(commanName, "Close") == 0) {
        command = cJSON_GetArrayItem(commands, 1);
    } else {
        ESP_LOGE(TAG, "Comando no existe %s", commanName);
        return NULL;
    }
    return cJSON_GetObjectItemCaseSensitive(command, "content")->valuestring;
}

char *getCommandsFromJSON(const char *name, const char *commanName) {
    cJSON *json = getDeviceFromJson(name);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist: %s", name);
        cJSON_Delete(json);
        return NULL;
    }

    char *deviceType = malloc(20 * sizeof(char));

    strcpy(deviceType, cJSON_GetObjectItemCaseSensitive(json, "type")->valuestring);

    switch (get_command_type(deviceType)) {
        case Garage:
            ESP_LOGI(TAG, "Getting garage command %s, for %s ", commanName, name);
            cJSON *commands = cJSON_GetObjectItemCaseSensitive(json, "commands");
            return getGarageCommand(name, commanName, json, commands);
        case ALARM:
        case LIGHT:
            ESP_LOGI(TAG, "Getting alarm command %s, for %s ", name, commanName);
            break;
        default: ;
    }
    return NULL;
}


int getDeviceID(char *name) {
    cJSON *json = getDeviceFromJson(name);
    if (json == NULL) {
        ESP_LOGE(TAG, "File does not exist: %s", name);
        cJSON_Delete(json);
        return NULL;
    }

    int deviceID = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;

    if (strlen(deviceID) == 0) {
        ESP_LOGE(TAG, "Not found device ID for: %s", name);
        cJSON_Delete(json);
        return NULL;
    }
    return deviceID;
}
