//
// Created by salman on 24/11/24.
//
#include <esp_log.h>

#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include <ui/MainScreen.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/CommonUI/Keyboard.h>
#include <ui/RFID/Utils/JSONManager/RfidDataStore.h>
#include <ui/Settings/SettingsScreen.h>

#include "AddRfidDispScreen.h"

static lv_obj_t *addRFIDInstance;

static lv_obj_t *keyboard;

void goToAddRfidScreen() {
    deletePreviousScreen(addRFIDInstance);
    ESP_LOGI("AddRfidScreen", "Going to AddRfidScreen");
    addrfiddispScreen();
    lv_scr_load(addRFIDInstance);
}


static void addrfiddispScreen() {
    addRFIDInstance = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(addRFIDInstance, lv_color_hex(0xc5d9db), 0);

    ESP_LOGI("addRfidScreen", "Changed to Add Rfid Dispositive");
    lv_obj_set_size(addRFIDInstance, 320, 240);
    lv_obj_set_style_bg_opa(addRFIDInstance, LV_OPA_COVER, 0);


    lv_obj_t *ta_name = lv_textarea_create(addRFIDInstance);
    lv_textarea_set_placeholder_text(ta_name, "Nombre del dispositivo");
    lv_textarea_set_max_length(ta_name, 10);
    lv_obj_set_width(ta_name, 140);
    lv_obj_set_height(ta_name, 50);
    lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 20, 40);
    lv_obj_add_event_cb(ta_name, textAreaHandler, LV_EVENT_ALL, NULL);

    createKeyboard(addRFIDInstance);



    lv_obj_t *btn_save = lv_btn_create(addRFIDInstance);
    lv_obj_set_size(btn_save, 80, 30);
    lv_obj_set_style_bg_color(btn_save, lv_color_hex(0x00bb2d), 0);
    lv_obj_align(btn_save, LV_ALIGN_BOTTOM_LEFT, 210, -150);

    lv_obj_t *btn_label_save = lv_label_create(btn_save);
    lv_label_set_text(btn_label_save, "Guardar");
    lv_obj_center(btn_label_save);

    lv_obj_t **widgets = malloc(1 * sizeof(lv_obj_t *));
    widgets[0] = ta_name;

    lv_obj_add_event_cb(btn_save, saveRFIDDispositives, LV_EVENT_CLICKED, widgets);


    lv_obj_t *btn_back = lv_btn_create(addRFIDInstance);
    lv_obj_set_size(btn_back, 80, 30);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0xff0000), 0);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 210, -100);

    lv_obj_t *btn_label_back = lv_label_create(btn_back);
    lv_label_set_text(btn_label_back, "Volver");
    lv_obj_center(btn_label_back);
    lv_obj_add_event_cb(btn_back, goToSettings, LV_EVENT_CLICKED, NULL);
}
