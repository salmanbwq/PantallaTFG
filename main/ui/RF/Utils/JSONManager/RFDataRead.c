//
// Created by salman on 11/02/25.
//

#include "RFDataRead.h"

#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <misc/lv_types.h>
#include <ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
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
        return NULL;
    }

    char *deviceType = malloc(20 * sizeof(char));

    strcpy(deviceType, cJSON_GetObjectItemCaseSensitive(json, "type")->valuestring);
    if (strlen(deviceType) == 0) {
        ESP_LOGE(TAG, "Not found type for: %s", name);
        cJSON_Delete(json);
        return NULL;
    }
    cJSON_Delete(json);
    return deviceType;
}


static char *getGarageCommand(const char *name, const char *commanName, cJSON *json, cJSON *commands) {
    if (commands == NULL) {
        ESP_LOGE(TAG, "No command exist for %s", name);
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

static char *getAlarmCommand(const char *name, const char *commanName, cJSON *json, cJSON *commands) {
    if (commands == NULL) {
        ESP_LOGE(TAG, "No command exist for %s", name);
        cJSON_Delete(json);
        return NULL;
    }
    cJSON *command;
    if (strcmp(commanName, "ON") == 0) {
        command = cJSON_GetArrayItem(commands, 0);
    } else if (strcmp(commanName, "OFF") == 0) {
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
            cJSON *commands = cJSON_GetObjectItemCaseSensitive(json, "command");
            return getGarageCommand(name, commanName, json, commands);
            break;
        case ALARM:
            ESP_LOGI(TAG, "Getting alarm command %s, for %s ", name, commanName);
            cJSON *commandsAlarmn = cJSON_GetObjectItemCaseSensitive(json, "command");
            return getAlarmCommand(name, commanName, json, commandsAlarmn);
            break;
        default: ;
    }
    cJSON_Delete(json);
    return NULL;
}
