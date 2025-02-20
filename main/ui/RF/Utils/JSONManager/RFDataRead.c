//
// Created by salman on 11/02/25.
//

#include "RFDataRead.h"

#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <misc/lv_types.h>
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


const char *getDeviceIDFromJson(const char *deviceName) {
    FILE *file = fopen("/spiffs/rf_devices.json", "r");
    if (!file) {
        ESP_LOGE("SPIFFS", "No se pudo abrir el archivo JSON");
        return NULL;
    }

    char buffer[1024] = {0}; // Asegurar buffer limpio
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, file);
    fclose(file);

    buffer[bytesRead] = '\0'; // Asegurar terminación de string

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        ESP_LOGE("SPIFFS", "Error al parsear JSON.");
        return NULL;
    }

    static char deviceId[32]; // Buffer estático para devolver la respuesta limpia
    int count = cJSON_GetArraySize(json);

    for (int i = 0; i < count; i++) {
        cJSON *device = cJSON_GetArrayItem(json, i);

        cJSON *nameObj = cJSON_GetObjectItem(device, "name");
        cJSON *id = cJSON_GetObjectItem(device, "id");

        if (!nameObj || !id || !cJSON_IsString(nameObj) || !cJSON_IsString(id)) {
            ESP_LOGE("SPIFFS", "Error: Campo 'name' o 'type' inválido en JSON.");
            continue;
        }

        if (strcmp(nameObj->valuestring, deviceName) == 0) {
            strncpy(deviceId, id->valuestring, sizeof(deviceId) - 1);
            deviceId[sizeof(deviceId) - 1] = '\0'; // Asegurar terminación de string
            cJSON_Delete(json);
            return deviceId;
        }
    }

    cJSON_Delete(json);
    return NULL;
}


CommandData *getCommandsFromJSON(const char *id) {
    static CommandData result; // Se usa static para que los datos no se pierdan
    memset(&result, 0, sizeof(result)); // Inicializar en caso de no encontrar datos

    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        ESP_LOGE(TAG, "No se pudo abrir el archivo JSON");
        return NULL;
    }

    char buffer[1024] = {0};
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, file);
    fclose(file);
    buffer[bytesRead] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        ESP_LOGE(TAG, "Error al parsear JSON.");
        return NULL;
    }

    int count = cJSON_GetArraySize(json);
    for (int i = 0; i < count; i++) {
        cJSON *device = cJSON_GetArrayItem(json, i);
        cJSON *idObj = cJSON_GetObjectItem(device, "id");
        cJSON *rfObj = cJSON_GetObjectItem(device, "rf");
        cJSON *commandsArr = cJSON_GetObjectItem(device, "commands");

        if (!idObj || !rfObj || !commandsArr ||
            !cJSON_IsString(idObj) || !cJSON_IsString(rfObj) ||
            !cJSON_IsArray(commandsArr)) {
            continue;
        }

        if (strcmp(idObj->valuestring, id) == 0) {
            strncpy(result.rf, rfObj->valuestring, sizeof(result.rf) - 1);

            int commandsCount = cJSON_GetArraySize(commandsArr);
            for (int j = 0; j < 2 && j < commandsCount; j++) {
                cJSON *command = cJSON_GetArrayItem(commandsArr, j);
                if (command && cJSON_IsString(command)) {
                    if (j == 0) {
                        strncpy(result.command1, command->valuestring, sizeof(result.command1) - 1);
                    } else {
                        strncpy(result.command2, command->valuestring, sizeof(result.command2) - 1);
                    }
                }
            }

            cJSON_Delete(json);
            return &result;
        }
    }

    cJSON_Delete(json);
    return NULL;
}
