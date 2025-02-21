//
// Created by salman on 8/02/25.
//

#include "RFDataStore.h"
#include <ui/Settings/AddRfDispScreen.h>
#include "cJSON/cJSON.h"
#include <esp_log.h>
#include <string.h>
#include <esp_spiffs.h>
#include <ui/MainScreen.h>
#include <ui/CommonUI/Keyboard.h>
#include <ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.h>
#include <ui/UILibs/CJSONStorage/JSONManager.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

#include "lvgl.h"
#define FILE_PATH "/spiffs/rf_devices.json"

static const char *TAG = "RFDataStore";


static void createGarageObj(cJSON *json) {
    cJSON *commands_array = cJSON_CreateArray();
    cJSON *openCmd = cJSON_CreateObject();
    cJSON *closeCmd = cJSON_CreateObject();
    cJSON_AddStringToObject(openCmd, "name", "Open"); // Nombre del comando
    cJSON_AddStringToObject(openCmd, "content", ""); // Contenido vacío por defecto
    cJSON_AddStringToObject(closeCmd, "name", "Close");
    cJSON_AddStringToObject(closeCmd, "content", "");
    cJSON_AddItemToArray(commands_array, openCmd);
    cJSON_AddItemToArray(commands_array, openCmd);
    cJSON_AddItemToObject(json, "command", commands_array);
}

static cJSON *getJson(const char *name, const char *type, const char *freq) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", name);
    cJSON_AddStringToObject(json, "freq", freq);
    cJSON_AddStringToObject(json, "type", type);

    switch (get_command_type(type)) {
        case Garage:
            ESP_LOGI(TAG, "Creating new Garage Dispositive");
            createGarageObj(json);
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
        AddDevices(getJson(ta_nameStr, typeStr, freqStr), FILE_PATH);
        showConfirmationPopup(lv_scr_act(), "Dispositivo guardado");
        // Borrar el cuadro de texto después de guardar
        lv_textarea_set_text(ta_name, "");
        ESP_LOGI(TAG, "New dispositive was added");
    } else {
        ESP_LOGI(TAG, "Dispositive can not be added");
    }
}
