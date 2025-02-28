//
// Created by salman on 23/01/25.
//

#include <ui/MainScreen.h>

#include "lvgl.h"
#include "SettingsScreen.h"

#include <esp_log.h>
#include <ui/CommonUI/InterfacesUtils.h>

#include "AddIR/AddIrDispScreen.h"
#include "AddRF/AddRfDispScreen.h"
#include "AddRFID/AddRfidDispScreen.h"
#include "Reset/Reset.h"

static lv_obj_t *settingInstance;

void goToSettings(lv_event_t *event) {
    deletePreviousScreen(settingInstance);
    ESP_LOGI("SettingsScreen", "Going to settings screen");
    settingsScreen();
    lv_scr_load(settingInstance);
}

void settingsScreen() {
    settingInstance = lv_obj_create(NULL);

    lv_obj_t *addRfDisp = lv_btn_create(settingInstance);
    lv_obj_t *rtrnMain = lv_btn_create(settingInstance);
    lv_obj_t *deleteJson = lv_btn_create(settingInstance);
    lv_obj_t *addRfidDisp = lv_btn_create(settingInstance);
    lv_obj_t *addIrDisp = lv_btn_create(settingInstance);

    lv_obj_set_size(settingInstance, 150, 50);
    lv_obj_set_pos(settingInstance, 75, 100);

    lv_obj_set_size(addRfDisp, 150, 50);
    lv_obj_set_pos(addRfDisp, 75, 30);
    lv_obj_t *rfAddDispFlb = lv_label_create(addRfDisp);
    lv_label_set_text(rfAddDispFlb, "Add RF dispositive");
    lv_obj_center(rfAddDispFlb);
    lv_obj_add_event_cb(addRfDisp, goToAddRfDispScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_set_size(addRfidDisp, 150, 50);
    lv_obj_set_pos(addRfidDisp, 75, 100);
    lv_obj_t *addRfidDispLbl = lv_label_create(addRfidDisp);
    lv_label_set_text(addRfidDispLbl, "Add Rfid dispositive");
    lv_obj_center(addRfidDispLbl);
    lv_obj_add_event_cb(addRfidDisp, goToAddRfidScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_set_size(addIrDisp, 150, 50);
    lv_obj_set_pos(addIrDisp, 75, 170);
    lv_obj_t *addIrDispLbl = lv_label_create(addIrDisp);
    lv_label_set_text(addIrDispLbl, "Add IR dispositive");
    lv_obj_center(addIrDispLbl);
    lv_obj_add_event_cb(addIrDisp, goToAddIrDispScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_set_size(deleteJson, 150, 50);
    lv_obj_set_pos(deleteJson, 75, 240);
    lv_obj_set_style_bg_color(deleteJson, lv_color_hex(0xff0000), 0);
    lv_obj_t *deleteJsonLbl = lv_label_create(deleteJson);
    lv_label_set_text(deleteJsonLbl, "Delete Json");
    lv_obj_center(deleteJsonLbl);
    lv_obj_add_event_cb(deleteJson, goToResetScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_set_size(rtrnMain, 150, 50);
    lv_obj_set_pos(rtrnMain, 75, 310);
    lv_obj_set_style_bg_color(rtrnMain, lv_color_hex(0xff0000), 0);
    lv_obj_t *rtrnMainLbl = lv_label_create(rtrnMain);
    lv_label_set_text(rtrnMainLbl, "Return");
    lv_obj_center(rtrnMainLbl);
    lv_obj_add_event_cb(rtrnMain, goToMainScreen, LV_EVENT_CLICKED, NULL);
}
