//
// Created by salman on 8/02/25.
//

#include "RFDataStore.h"
#include "cJSON/cJSON.h"
#include <esp_log.h>
#include <string.h>
#include <esp_spiffs.h>
#include <ui/MainScreen.h>
#include <ui/CommonUI/Keyboard.h>
#include <ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.h>
#include <ui/UILibs/CJSONStorage/JSONManager.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/CJSONStorage/Write/WriteJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

#include "lvgl.h"
#define FILE_PATH "/spiffs/rf_devices.json"

static const char *TAG = "RFDataStore";


static cJSON *createGarageObj() {
    cJSON *commands_array = cJSON_CreateArray();
    cJSON *openCmd = cJSON_CreateObject();
    cJSON *closeCmd = cJSON_CreateObject();
    cJSON_AddStringToObject(openCmd, "name", "Open"); // Nombre del comando
    cJSON_AddStringToObject(openCmd, "content", ""); // Contenido vacío por defecto
    cJSON_AddStringToObject(closeCmd, "name", "Close");
    cJSON_AddStringToObject(closeCmd, "content", "");
    cJSON_AddItemToArray(commands_array, openCmd);
    cJSON_AddItemToArray(commands_array, closeCmd);
    ESP_LOGI(TAG, "Garage objects created");
    return commands_array;
}

static cJSON *getJson(const char *name, const char *type, const char *freq) {

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", name);
    cJSON_AddStringToObject(json, "freq", freq);
    cJSON_AddStringToObject(json, "type", type);

    switch (get_command_type(type)) {
        case Garage:
            ESP_LOGI(TAG, "Creating new Garage Dispositive");
            cJSON_AddItemToObject(json, "command", createGarageObj());
            break;

        case ALARM:
            ESP_LOGI(TAG, "Creating new Alarm Dispositive");
            cJSON_AddBoolToObject(json, "state", false);
            break;

        case LIGHT:
            ESP_LOGI(TAG, "Creating new Light Dispositive");
            cJSON_AddBoolToObject(json, "state", false);
            break;

        default: ESP_LOGE(TAG, "Unknown type");
    }


    return json;
}

void saveRfDispositives(lv_event_t *indev) {
    lv_obj_t **widget = (lv_obj_t **) lv_event_get_user_data(indev);
    if (!widget) {
        ESP_LOGE("TAG", "User data is NULL");
        return;
    }

    lv_obj_t *ta_name = widget[1];
    lv_obj_t *type = widget[0];
    lv_obj_t *freq = widget[2];

    if (!ta_name || !type || !freq) {
        ESP_LOGE(TAG, "Input data is NULL");
        return;
    }

    char typeStr[50];
    char freqStr[20];
    char ta_nameStr[100];

    strcpy(ta_nameStr, lv_textarea_get_text(ta_name));
    lv_dropdown_get_selected_str(type, typeStr, sizeof(typeStr));
    lv_dropdown_get_selected_str(freq, freqStr, sizeof(freqStr));

    if (strcmp(typeStr, "") != 0 && strcmp(freqStr, "") != 0 && strcmp(typeStr, "") != 0) {
        cJSON *json = getJson(ta_nameStr, typeStr, freqStr);
        if (!json) {
            ESP_LOGE(TAG, "JSON created is NULL");
            return;
        }
        AddDevices(json, FILE_PATH);
        showConfirmationPopup(lv_scr_act(), "Dispositivo guardado");
        // Borrar el cuadro de texto después de guardar
        lv_textarea_set_text(ta_name, "");
        ESP_LOGI(TAG, "New dispositive was added");
    } else {
        ESP_LOGI(TAG, "Dispositive can not be added");
    }
}

esp_err_t updateRFJSON(char *name, const char *commandName, char *content) {
    cJSON *file = getJsonByName(name, FILE_PATH);
    if (!file) {
        ESP_LOGE(TAG, "No se encontró el JSON para el nombre: %s", name);
        return ESP_FAIL;
    }

    cJSON *command_array = cJSON_GetObjectItem(file, "command");
    if (!cJSON_IsArray(command_array)) {
        ESP_LOGE(TAG, "El campo 'command' no es un array.");
        cJSON_Delete(file);
        return ESP_FAIL;
    }

    int count = cJSON_GetArraySize(command_array);
    bool updated = false;

    for (int i = 0; i < count; i++) {
        cJSON *cmd_obj = cJSON_GetArrayItem(command_array, i);
        cJSON *cmd_name = cJSON_GetObjectItem(cmd_obj, "name");

        if (cJSON_IsString(cmd_name) && strcmp(cmd_name->valuestring, commandName) == 0) {
            // Encontramos un objeto con "name" vacío, lo actualizamos
            cJSON_ReplaceItemInObject(cmd_obj, "content", cJSON_CreateString(content));

            updated = true;
            ESP_LOGI(TAG, "Comando en la posición %d actualizado: {name: %s, content: %s}", i, commandName, content);
            break;
        }
    }

    if (!updated) {
        ESP_LOGW(TAG, "Todos los comandos están llenos, no se pudo actualizar.");
        cJSON_Delete(file);
    } else {
        updateDevicesListJson(file, FILE_PATH); // Guardar cambios
        cJSON_Delete(file);
        return ESP_OK;
    }

    return ESP_FAIL;
}

