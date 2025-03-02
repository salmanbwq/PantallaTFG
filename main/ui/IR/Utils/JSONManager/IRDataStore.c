//
// Created by salman on 21/02/25.
//

#include "IRDataStore.h"

#include <esp_err.h>
#include <esp_log.h>
#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include <cJSON/cJSON.h>
#include <ui/IR/Utils/DispositiveSelector/IRDispositiveSelector.h>
#include <ui/UILibs/CJSONStorage/JSONManager.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/CJSONStorage/Write/WriteJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>
static const char *TAG = "IRDataRead";
static const char *FILE_PATH = "/spiffs/irDevices.json";

static void createTVObj(cJSON *json) {
    cJSON *commands_array = cJSON_CreateArray();

    // Inicializar con comandos vacíos
    for (int i = 0; i < 7; i++) {
        cJSON *cmd_obj = cJSON_CreateObject();
        char commandName[20]; // Cadena con suficiente espacio
        sprintf(commandName, "Command %d", i + 1);
        cJSON_AddStringToObject(cmd_obj, "name", commandName); // Nombre del comando
        cJSON_AddStringToObject(cmd_obj, "content", ""); // Contenido vacío por defecto
        cJSON_AddItemToArray(commands_array, cmd_obj);
    }

    cJSON_AddItemToObject(json, "command", commands_array);
}

static void createLampObj(cJSON *json) {
    cJSON *commands_array = cJSON_CreateArray();

    // Inicializar con comandos vacíos
    for (int i = 0; i < 2; i++) {
        cJSON *cmd_obj = cJSON_CreateObject();
        char commandName[20]; // Cadena con suficiente espacio
        sprintf(commandName, "Command %d", i + 1);
        cJSON_AddStringToObject(cmd_obj, "name", commandName); // Nombre del comando
        cJSON_AddStringToObject(cmd_obj, "content", ""); // Contenido vacío por defecto
        cJSON_AddItemToArray(commands_array, cmd_obj);
    }

    cJSON_AddItemToObject(json, "command", commands_array);
}


static void createACObj(cJSON *json) {
    cJSON *commands_array = cJSON_CreateArray();

    // Inicializar con comandos vacíos
    for (int i = 0; i < 4; i++) {
        cJSON *cmd_obj = cJSON_CreateObject();
        char commandName[20]; // Cadena con suficiente espacio
        sprintf(commandName, "Command %d", i + 1);
        cJSON_AddStringToObject(cmd_obj, "name", commandName); // Nombre del comando
        cJSON_AddStringToObject(cmd_obj, "content", ""); // Contenido vacío por defecto
        cJSON_AddItemToArray(commands_array, cmd_obj);
    }

    cJSON_AddItemToObject(json, "command", commands_array);
}

static cJSON *getJson(const char *name, const char *type) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", name);
    cJSON_AddStringToObject(json, "type", type);

    switch (getIRType(type)) {
        case TV:
            ESP_LOGI(TAG, "Creating new %s Dispositive", type);
            createTVObj(json);
            break;

        case A_C:
            ESP_LOGI(TAG, "Creating new %s Dispositive", type);
            createACObj(json);
            break;
        case LAMP:
            ESP_LOGI(TAG, "Creating new %s Dispositive", type);
            createLampObj(json);
            break;

        default: ESP_LOGE(TAG, "Unknown type");
            return NULL;
    }


    return json;
}


void saveIRDispositives(lv_event_t *indev) {
    lv_obj_t **widget = (lv_obj_t **) lv_event_get_user_data(indev);
    if (!widget) {
        ESP_LOGE("TAG", "User data is NULL");
        return;
    }

    lv_obj_t *ta_name = widget[1];
    lv_obj_t *type = widget[0];

    if (!ta_name || !type) {
        ESP_LOGE(TAG, "Input data is NULL");
        return;
    }

    char typeStr[50];
    char ta_nameStr[100];

    strcpy(ta_nameStr, lv_textarea_get_text(ta_name));
    lv_dropdown_get_selected_str(type, typeStr, sizeof(typeStr));

    if (strcmp(ta_nameStr, "") != 0 && strcmp(typeStr, "") != 0) {
        cJSON *json = getJson(ta_nameStr, typeStr);
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

esp_err_t updateIRJSON(char *name, const char *commandName, char *content) {
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
            // Borrar y agregar nuevo contenido
            cJSON_DeleteItemFromObject(cmd_obj, "content");
            cJSON_AddStringToObject(cmd_obj, "content", content);
            updated = true;
            ESP_LOGI(TAG, "Comando actualizado: {name: %s, content: %s}", commandName, content);
            break;
        }
    }

    if (!updated) {
        ESP_LOGW(TAG, "No se pudo actualizar ningún comando. Es posible que el nombre no coincida.");
    }

    updateDevicesListJson(file, FILE_PATH); // Guardar cambios
    cJSON_Delete(file);
    return updated ? ESP_OK : ESP_FAIL;
}
