//
// Created by salman on 24/11/24.
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lvgl.h"
#include "SendRfScreen.h"
#include "esp_spiffs.h"
#include <esp_log.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/RF/RfScreen.h>
#include <ui/RF/Utils/Enums.h>
#include <ui/RF/Utils/Dropdown/RFDropDown.h>
#include <ui/Settings/SettingsScreen.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>


#define FILE_PATH "/spiffs/rf_devices.json"
static const char *TAG = "SendRFScreen";
static lv_obj_t *sendRfScrn;


void goToSendRfScreen(lv_event_t *event) {
    deletePreviousScreen(sendRfScrn);
    ESP_LOGI(TAG, "Going to Sending RfScreen");
    sendRfScreen();
    lv_scr_load(sendRfScrn);
}


static void sendRfScreen(void) {
    static DispositiveSelectorType dispositive_selector = SENDER;
    sendRfScrn = lv_obj_create(NULL);
    lv_obj_t *rtrngrg = lv_btn_create(sendRfScrn);
    lv_obj_set_size(sendRfScrn, 150, 50);
    lv_obj_set_pos(sendRfScrn, 75, 100);
    // Dropdown para el tipo de dispositivo
    lv_obj_t *dd_type = lv_dropdown_create(sendRfScrn);
    populateDropdownNames(dd_type, FILE_PATH);

    if (lv_dropdown_get_option_count(dd_type) == 0) {
        showConfirmationPopup(sendRfScrn, "Data empty");
        lv_obj_add_state(dd_type, LV_STATE_DISABLED);
    }


    lv_obj_set_width(dd_type, 150); // Ajustar ancho
    lv_obj_set_pos(dd_type, 75, 30); // Posición en la esquina superior izquierda
    lv_obj_add_event_cb(dd_type, dropdownEventHandlerRF, LV_EVENT_ALL, &dispositive_selector);

    lv_obj_set_size(rtrngrg, 150, 50);
    lv_obj_set_pos(rtrngrg, 75, 100);
    lv_obj_set_style_bg_color(rtrngrg, lv_color_hex(0xff0000), 0);
    lv_obj_t *rtrnLbl = lv_label_create(rtrngrg);
    lv_label_set_text(rtrnLbl, "Return");
    lv_obj_center(rtrnLbl);
    lv_obj_add_event_cb(rtrngrg, goToRfScreen, LV_EVENT_CLICKED, NULL);
}
