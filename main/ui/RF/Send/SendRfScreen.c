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
    lv_obj_t *returnBtn = lv_btn_create(sendRfScrn);
    lv_obj_set_size(sendRfScrn, 150, 50);
    lv_obj_set_pos(sendRfScrn, 75, 100);
    // Dropdown para el tipo de dispositivo
    lv_obj_t *dd_type = lv_dropdown_create(sendRfScrn);
    populateDropdownNames(dd_type, FILE_PATH);
    lv_obj_align(dd_type, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_size(dd_type, 189, 40);

    lv_obj_t *title = lv_label_create(sendRfScrn);
    lv_label_set_text(title, "Send RF");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x333333), 0);

    lv_obj_t *dropdownLabel = lv_label_create(sendRfScrn);
    lv_label_set_text(dropdownLabel, "Select Dispositive");
    lv_obj_align(dropdownLabel, LV_ALIGN_CENTER, 0, -60);
    lv_obj_set_style_text_font(dropdownLabel, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(dropdownLabel, lv_color_hex(0x555555), 0);

    if (lv_dropdown_get_option_count(dd_type) == 0) {
        showConfirmationPopup(sendRfScrn, "Data empty");
        lv_obj_add_state(dd_type, LV_STATE_DISABLED);
    }

    lv_obj_add_event_cb(dd_type, dropdownEventHandlerRF, LV_EVENT_ALL, &dispositive_selector);


    lv_obj_set_style_radius(dd_type, 10, 0); // Bordes redondeados
    lv_obj_set_style_bg_color(dd_type, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_color(dd_type, lv_color_hex(0xBBBBBB), 0);
    lv_obj_set_style_border_width(dd_type, 2, 0);
    lv_obj_set_style_shadow_width(dd_type, 10, 0);
    lv_obj_set_style_shadow_color(dd_type, lv_color_hex(0xDDDDDD), 0);


    lv_obj_set_size(returnBtn, 160, 40);
    lv_obj_align(returnBtn, LV_ALIGN_BOTTOM_MID, 0, -50);

    lv_obj_set_style_bg_color(returnBtn, lv_color_hex(0xD32F2F), 0); // Rojo oscuro
    lv_obj_set_style_radius(returnBtn, 10, 0); // Bordes redondeados
    lv_obj_set_style_shadow_width(returnBtn, 10, 0); // Sombra para efecto 3D
    lv_obj_set_style_shadow_color(returnBtn, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_bg_color(returnBtn, lv_color_hex(0xB71C1C), LV_STATE_PRESSED); // Cambio de color al presionar

    lv_obj_t *returnBtnLabel = lv_label_create(returnBtn);
    lv_label_set_text(returnBtnLabel, "Return");
    lv_obj_set_style_text_color(returnBtnLabel, lv_color_hex(0xFFFFFF), 0); // Texto blanco
    lv_obj_set_style_text_font(returnBtnLabel, &lv_font_montserrat_18, 0);
    lv_obj_center(returnBtnLabel);

    lv_obj_add_event_cb(returnBtn, goToRfScreen, LV_EVENT_CLICKED, NULL);
}
