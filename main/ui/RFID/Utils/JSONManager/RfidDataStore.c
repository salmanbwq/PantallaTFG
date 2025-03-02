//
// Created by salman on 15/02/25.
//

#include "RfidDataStore.h"

#include "cJSON/cJSON.h"
#include <esp_log.h>
#include <string.h>
#include <ui/MainScreen.h>
#include <ui/UILibs/CJSONStorage/JSONManager.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/CJSONStorage/Write/WriteJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

#include "lvgl.h"
#define FILE_PATH "/spiffs/rfidDevices.json"

static const char *TAG = "RFIDDataStore";

static cJSON *getJson(const char *name) {
    cJSON *json = cJSON_CreateObject();
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

    cJSON_AddStringToObject(json, "name", name);
    cJSON_AddItemToObject(json, "command", commands_array);

    return json;
}

esp_err_t updateRFIDJSON(char *name, char *commandName, char *content) {
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

void saveRFIDDispositives(lv_event_t *indev) {
    lv_obj_t **widget = (lv_obj_t **) lv_event_get_user_data(indev);
    if (!widget) {
        ESP_LOGE("TAG", "User data is NULL");
        return;
    }

    lv_obj_t *ta_name = widget[0];

    if (!ta_name) {
        ESP_LOGE(TAG, "Name is NULL");
        return;
    }

    const char *name = lv_textarea_get_text(ta_name);


    if (strcmp(name, "") != 0) {
        AddDevices(getJson(name), FILE_PATH);
        showConfirmationPopup(lv_scr_act(), "Dispositivo guardado");

        // Borrar el cuadro de texto después de guardar
        lv_textarea_set_text(ta_name, "");
        ESP_LOGI(TAG, "New dispositive was added");
    } else {
        showConfirmationPopup(lv_scr_act(), "Error: Campos vacios");
        ESP_LOGE(TAG, "Dispositive name or type cannot be empty");
    }
}

static void updateDropDown(cJSON *file, lv_obj_t *dropdown) {
    lv_dropdown_clear_options(dropdown);
    if (!file) {
        ESP_LOGE(TAG, "Json vacio");
        return;
    }

    cJSON *command_array = cJSON_GetObjectItem(file, "command");
    if (!cJSON_IsArray(command_array)) {
        ESP_LOGE(TAG, "El campo 'command' no es un array.");
        cJSON_Delete(file);
        return;
    }

    int count = cJSON_GetArraySize(command_array);
    ESP_LOGI(TAG, "%d element(s) en la lista.", count);

    for (int i = 0; i < count; i++) {
        cJSON *cmd_obj = cJSON_GetArrayItem(command_array, i);
        char *cmd_name = cJSON_GetObjectItem(cmd_obj, "name")->valuestring;
        ESP_LOGI(TAG, "Name: %s", cmd_name);

        lv_dropdown_add_option(dropdown, cmd_name, i);
    }
    lv_obj_clear_state(dropdown, LV_STATE_DISABLED);
    ESP_LOGI(TAG, "Dropdown actualizado con %d dispositivos.", count);
    cJSON_Delete(file);
}


void populateDropdownCommands(lv_event_t *indev) {
    // Limpiar dropdown antes de agregar nuevos elementos
    if (lv_event_get_code(indev) == LV_EVENT_VALUE_CHANGED) {
        ESP_LOGI(TAG, "Nueva opcion seleccionada");
        lv_obj_t *dropdw = lv_event_get_target(indev);
        lv_obj_t *commandDropdown = lv_event_get_user_data(indev);
        if (!dropdw || !commandDropdown) {
            ESP_LOGE(TAG, "Evento dropped no target");
            return;
        }
        char selectedOption[100];
        lv_dropdown_get_selected_str(dropdw, selectedOption, sizeof(selectedOption));
        cJSON *file = getJsonByName(selectedOption, FILE_PATH);
        updateDropDown(file, commandDropdown);
    }
}
