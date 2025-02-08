//
// Created by salman on 8/02/25.
//

#include "RFDataStore.h"
#include <ui/Settings/AddRfDispScreen.h>

#include <esp_log.h>
#include <string.h>
#include <ui/MainScreen.h>
#include <ui/GeneralObjects/Keyboard.h>

#include "lvgl.h"

void saveRfDispositives(lv_event_t *indev) {
    if (rfDispCount >= MAX_DEVICES) {
        ESP_LOGE("addRfidScreen", "Memory is full");
        return;
    }

    // Recuperar el user_data correctamente
    lv_obj_t **widget = (lv_obj_t **) lv_event_get_user_data(indev);
    if (!widget) {
        ESP_LOGE("saveRfidDispositives", "User data is NULL");
        return;
    }

    lv_obj_t *dd_type = widget[0];
    lv_obj_t *ta_name = widget[1];

    if (!dd_type || !ta_name) {
        ESP_LOGE("saveRfidDispositives", "One of the widgets is NULL -> dd_type: %p, ta_name: %p", dd_type, ta_name);
        return;
    }

    // Imprimir los valores antes de usarlos para debug
    ESP_LOGI("saveRfidDispositives", "Dropdown address: %p, Textarea address: %p", dd_type, ta_name);
    ESP_LOGI("saveRfidDispositives", "Textarea text: %s", lv_textarea_get_text(ta_name));

    // Cerrar el teclado antes de leer el texto
    if (keyboard) {
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }

    // Obtener el texto correctamente
    const char *name = lv_textarea_get_text(ta_name);
    strncpy(devicesRf[rfDispCount].name, name, sizeof(devicesRf[rfDispCount].name));

    // Obtener el tipo seleccionado
    lv_dropdown_get_selected_str(dd_type, devicesRf[rfDispCount].type, sizeof(devicesRf[rfDispCount].type));

    if (strcmp(devicesRf[rfDispCount].type, "") != 0 && strcmp(devicesRf[rfDispCount].name, "") != 0) {
        rfDispCount++;
        ESP_LOGI("addRfidScreen", "New dispositive was added, available space %d", MAX_DEVICES - rfDispCount);
    } else {
        ESP_LOGE("addRfidScreen", "Dispositive name or type cannot be empty");
        return;
    }
}
