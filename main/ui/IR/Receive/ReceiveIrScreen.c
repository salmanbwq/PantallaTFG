//
// Created by salman on 25/01/25.
//

#include <esp_log.h>
#include <string.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/IR/IRScreen.h>
#include <ui/IR/Utils/Enums.h>
#include <ui/IR/Utils/DispositiveSelector/IRDispositiveSelector.h>
#include <ui/IR/Utils/DropDown/IRDropdown.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

#include "lvgl.h"
#include "ReceiveIRScreen.h"

static char *FILE_PATH = "/spiffs/irDevices.json";
static lv_obj_t *receiveIRInstance;

void goToReceiveIrScreen() {
    deletePreviousScreen(receiveIRInstance);
    ESP_LOGI("IRScreen", "Going to receive IR screen");
    receiveIrScreen();
    lv_scr_load(receiveIRInstance);
}


static void receiveIrScreen() {
    static IRActionType action = RECEIVER;
    receiveIRInstance = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(receiveIRInstance, lv_color_hex(0xc5d9db), 0);

    lv_obj_t *title = lv_label_create(receiveIRInstance);
    lv_label_set_text(title, "Receive IR");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x333333), 0);

    lv_obj_t *dropdownLabel = lv_label_create(receiveIRInstance);
    lv_label_set_text(dropdownLabel, "Select Dispositive");
    lv_obj_align(dropdownLabel, LV_ALIGN_CENTER, 0, -60);
    lv_obj_set_style_text_font(dropdownLabel, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(dropdownLabel, lv_color_hex(0x555555), 0);

    lv_obj_t *dispositiveNameDropdown = lv_dropdown_create(receiveIRInstance);
    lv_obj_add_event_cb(dispositiveNameDropdown, dropdownEventHandler, LV_EVENT_VALUE_CHANGED, &action);

    lv_obj_align(dispositiveNameDropdown, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_size(dispositiveNameDropdown, 189, 40);
    populateDropdownNames(dispositiveNameDropdown, FILE_PATH);

    if (lv_dropdown_get_option_count(dispositiveNameDropdown) == 0) {
        showConfirmationPopup(receiveIRInstance, "Data empty");
        lv_obj_add_state(dispositiveNameDropdown, LV_STATE_DISABLED);
    }

    lv_obj_set_style_radius(dispositiveNameDropdown, 10, 0); // Bordes redondeados
    lv_obj_set_style_bg_color(dispositiveNameDropdown, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_color(dispositiveNameDropdown, lv_color_hex(0xBBBBBB), 0);
    lv_obj_set_style_border_width(dispositiveNameDropdown, 2, 0);
    lv_obj_set_style_shadow_width(dispositiveNameDropdown, 10, 0);
    lv_obj_set_style_shadow_color(dispositiveNameDropdown, lv_color_hex(0xDDDDDD), 0);

    lv_obj_t *returnBtn = lv_btn_create(receiveIRInstance);
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
    lv_obj_add_event_cb(returnBtn, goToIRScreen, LV_EVENT_CLICKED,NULL);
}
