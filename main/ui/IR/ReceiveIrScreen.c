//
// Created by salman on 25/01/25.
//

#include <esp_log.h>
#include <ui/CommonUI/InterfacesUtils.h>

#include "IRScreen.h"
#include "lvgl.h"
#include "ReceiveIRScreen.h"

void goToReceiveIrScreen(lv_indev_t *indev) {
    deletePreviousScreen(receiveIrScrn);
    ESP_LOGI("IRScreen", "Going to receive IR screen");
    receiveIrScreen();
    lv_scr_load(receiveIrScrn);
}

static void receiveIrScreen() {
    receiveIrScrn = lv_obj_create(NULL);

    lv_obj_t *dddisp = lv_dropdown_create(receiveIrScrn);
    lv_dropdown_set_options(dddisp, "Tv\nA/C\nXbox\nHumidity Sensor");
    lv_obj_set_pos(dddisp, 40, 30);
    lv_obj_set_size(dddisp, 100, 30);

    lv_obj_t *ddcommand = lv_dropdown_create(receiveIrScrn);
    lv_dropdown_set_options(ddcommand, "On\nOff\n+\n-\n");
    lv_obj_set_size(ddcommand, 100, 30);
    lv_obj_set_pos(ddcommand, 40, 80);

    lv_obj_t *readBtn = lv_btn_create(receiveIrScrn);
    lv_obj_t *readBtnLable = lv_label_create(readBtn);
    lv_label_set_text(readBtnLable, "Receive");
    lv_obj_center(readBtnLable);
    lv_obj_set_style_bg_color(readBtn, lv_color_hex(0x00bb2d), 0);
    lv_obj_set_size(readBtn, 70, 30);
    lv_obj_set_pos(readBtn, 220, 50);

    lv_obj_t *returnBtn = lv_btn_create(receiveIrScrn);
    lv_obj_t *returnBtnLbl = lv_label_create(returnBtn);
    lv_label_set_text(returnBtnLbl, "Return");
    lv_obj_set_size(returnBtn, 70, 30);
    lv_obj_center(returnBtnLbl);
    lv_obj_set_style_bg_color(returnBtn, lv_color_hex(0xff0000), 0);
    lv_obj_set_pos(returnBtn, 220, 100);
    lv_obj_add_event_cb(returnBtn, goToIRScreen, LV_EVENT_CLICKED,NULL);
}
