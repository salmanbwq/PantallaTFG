//
// Created by salman on 25/01/25.
//

#include <esp_log.h>
#include <string.h>

#include "lvgl.h"
#include "SendIrScreen.h"

#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/IR/IRScreen.h>
#include <ui/IR/Utils/DispositiveSelector/IRDispositiveSelector.h>
#include <ui/IR/Utils/DropDown/IRDropdown.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>
#define FILE_PATH "/spiffs/irDevices.json"

static lv_obj_t *sendIRInstance;


void goToIrSend() {
    deletePreviousScreen(sendIRInstance);
    ESP_LOGI("IRScreen", "Going to send IR");
    sendIrScreen();
    lv_scr_load(sendIRInstance);
}


static void sendIrScreen() {
    static IRActionType action = SENDER;
    sendIRInstance = lv_obj_create(NULL);

    lv_obj_t *returnBtn = lv_btn_create(sendIRInstance);
    lv_obj_t *returnBtnLabel = lv_label_create(returnBtn);

    lv_obj_t *dispositiveNameDropdown = lv_dropdown_create(sendIRInstance);
    lv_obj_add_event_cb(dispositiveNameDropdown, dropdownEventHandler, LV_EVENT_VALUE_CHANGED, &action);

    lv_obj_set_pos(dispositiveNameDropdown, 100, 30);
    lv_obj_set_size(dispositiveNameDropdown, 100, 30);
    populateDropdownNames(dispositiveNameDropdown, FILE_PATH);

    if (lv_dropdown_get_option_count(dispositiveNameDropdown) == 0) {
        showConfirmationPopup(sendIRInstance, "Data empty");
        lv_obj_add_state(dispositiveNameDropdown, LV_STATE_DISABLED);
    }

    lv_obj_set_style_bg_color(returnBtn, lv_color_hex(0xff0000), 0);
    lv_label_set_text(returnBtnLabel, "Return");
    lv_obj_center(returnBtnLabel);

    lv_obj_set_pos(returnBtn, 75, 100);
    lv_obj_set_size(returnBtn, 150, 30);

    lv_obj_add_event_cb(returnBtn, goToIRScreen, LV_EVENT_CLICKED, NULL);
}
