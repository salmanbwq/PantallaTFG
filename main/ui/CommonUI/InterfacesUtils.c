//
// Created by salman on 8/02/25.
//

#include "InterfacesUtils.h"

#include <esp_log.h>

#include "lvgl.h"

void deletePreviousScreen(lv_obj_t *parent) {
    if (lv_scr_act() != parent) {
        ESP_LOGI("Screen Delete", "Deleting previous screen");
        lv_obj_del(lv_scr_act()); // Elimina la pantalla actual
    }
}
