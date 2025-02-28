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
#include <ui/IR/Utils/JSONManager/IRDataRead.h>
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

    lv_obj_t *dispositiveNameDropdown = lv_dropdown_create(receiveIRInstance);
    lv_obj_set_pos(dispositiveNameDropdown, 40, 30);
    lv_obj_set_size(dispositiveNameDropdown, 100, 30);

    populateDropdownNames(dispositiveNameDropdown, FILE_PATH);

    lv_obj_add_event_cb(dispositiveNameDropdown, dropdownEventHandler, LV_EVENT_VALUE_CHANGED, &action);

    if (lv_dropdown_get_option_count(dispositiveNameDropdown) == 0) {
        showConfirmationPopup(receiveIRInstance, "Data empty");
        lv_obj_add_state(dispositiveNameDropdown, LV_STATE_DISABLED);
    }

    lv_obj_t *returnBtn = lv_btn_create(receiveIRInstance);
    lv_obj_t *returnBtnLbl = lv_label_create(returnBtn);
    lv_label_set_text(returnBtnLbl, "Return");
    lv_obj_set_size(returnBtn, 70, 30);
    lv_obj_center(returnBtnLbl);
    lv_obj_set_style_bg_color(returnBtn, lv_color_hex(0xff0000), 0);
    lv_obj_set_pos(returnBtn, 220, 50);
    lv_obj_add_event_cb(returnBtn, goToIRScreen, LV_EVENT_CLICKED,NULL);
}
