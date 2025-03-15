//
// Created by salman on 24/02/25.
//

#include "Reset.h"

#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/Settings/SettingsScreen.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

#include "lvgl.h"

static const char *TAG = "Reset";
static lv_obj_t *resetScrn;

typedef enum JsonTypes {
    RF, RFID, IR, ALL, UNKNOWN
} JsonTypes;



JsonTypes getJsonName(const char *command) {
    if (strcmp(command, "Delete RF") == 0) return RF;
    if (strcmp(command, "Delete RFID") == 0) return RFID;
    if (strcmp(command, "Delete IR") == 0) return IR;
    if (strcmp(command, "Delete all") == 0) return ALL;
    return UNKNOWN;
}


void goToResetScreen() {
    deletePreviousScreen(resetScrn);
    ESP_LOGI("AddRfidScreen", "Going to AddRfidScreen");
    resetScreen();
    lv_scr_load(resetScrn);
}

static void deleteJsons(lv_event_t *event) {
    const lv_obj_t *dropdown = (lv_obj_t *) lv_event_get_user_data(event);
    if (dropdown == NULL) {
        ESP_LOGE(TAG, "Dropdown is NULL");
        return;
    }
    char jsonName[100];
    lv_dropdown_get_selected_str(dropdown, jsonName, sizeof(jsonName));
    if (strlen(jsonName) == 0) {
        ESP_LOGI(TAG, "No Json selected");
        return;
    }

    switch (getJsonName(jsonName)) {
        case RF:
            ESP_LOGI(TAG, "Deleting RF Json");
            remove("/spiffs/rf_devices.json");
            showConfirmationPopup(resetScrn, "RF Json Deleted");
            break;
        case RFID:
            ESP_LOGI(TAG, "Deleting RFID Json");
            remove("/spiffs/rfidDevices.json");
            showConfirmationPopup(resetScrn, "RFID Json Deleted");
            break;
        case IR:
            ESP_LOGI(TAG, "Deleting IR Json");
            remove("/spiffs/irDevices.json");
            showConfirmationPopup(resetScrn, "IR Json Deleted");
            break;
        case ALL:
            ESP_LOGI(TAG, "Deleting all Json");
            remove("/spiffs/rfidDevices.json");
            remove("/spiffs/rf_devices.json");
            remove("/spiffs/irDevices.json");
            showConfirmationPopup(resetScrn, "All Json Deleted");
            break;
        default:
            ESP_LOGI(TAG, "Unknown Json selected");
    }
}


static void resetScreen() {
    resetScrn = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(resetScrn, lv_color_hex(0xc5d9db), 0);

    lv_obj_t *jsonDropdown = lv_dropdown_create(resetScrn);
    lv_obj_t *deleteBtn = lv_btn_create(resetScrn);
    lv_obj_t *returnBtn = lv_btn_create(resetScrn);
    lv_obj_t *deleteBtnLbl = lv_label_create(deleteBtn);
    lv_obj_t *returnBtnLbl = lv_label_create(returnBtn);
    lv_dropdown_clear_options(jsonDropdown);

    lv_obj_set_pos(jsonDropdown, 20, 30);
    lv_obj_set_size(jsonDropdown, 165, 40);
    lv_dropdown_add_option(jsonDropdown, "Delete RF", 0);
    lv_dropdown_add_option(jsonDropdown, "Delete RFID", 1);
    lv_dropdown_add_option(jsonDropdown, "Delete IR", 2);
    lv_dropdown_add_option(jsonDropdown, "Delete all", 3);

    lv_label_set_text(deleteBtnLbl, "Delete");
    lv_obj_center(deleteBtnLbl);

    lv_obj_set_pos(deleteBtn, 220, 50);
    lv_obj_set_style_bg_color(deleteBtn, lv_color_hex(0x008f39), 0);
    lv_obj_set_size(deleteBtn, 70, 30);

    lv_label_set_text(returnBtnLbl, "Return");
    lv_obj_center(returnBtnLbl);

    lv_obj_set_pos(returnBtn, 220, 100);
    lv_obj_set_style_bg_color(returnBtn, lv_color_hex(0xff0000), 0);
    lv_obj_set_size(returnBtn, 70, 30);

    lv_obj_add_event_cb(deleteBtn, deleteJsons, LV_EVENT_PRESSED, (lv_obj_t *) jsonDropdown);
    lv_obj_add_event_cb(returnBtn, goToSettings, LV_EVENT_CLICKED, NULL);
}
