//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "LightRfScreen.h"

#include <ui/RF/Send/SendRfScreen.h>


void ligthRfScreen() {
    lightRfScrn = lv_obj_create(NULL);

    lv_obj_t *btnOn = lv_btn_create(lightRfScrn);
    lv_obj_t *rtrnbtn = lv_btn_create(lightRfScrn);
    lv_obj_t *labelBtnOn = lv_label_create(btnOn);
    lv_obj_t *rtrnbtnLabel = lv_label_create(rtrnbtn);


    lv_label_set_text(labelBtnOn, "On");
    lv_obj_set_pos(btnOn, 120, 30);
    lv_obj_set_size(btnOn, 50, 50);

    lv_obj_t *btnOff = lv_btn_create(lightRfScrn);
    lv_obj_t *labelBtnOff = lv_label_create(btnOff);
    lv_obj_center(labelBtnOff);
    lv_obj_center(labelBtnOn);
    lv_label_set_text(labelBtnOff, "Off");
    lv_obj_set_size(btnOff, 50, 50);
    lv_obj_set_pos(btnOff, 120, 100);


    lv_obj_set_size(rtrnbtn, 150, 50);
    lv_obj_set_pos(rtrnbtn, 70, 170);
    lv_obj_set_style_bg_color(rtrnbtn, lv_color_hex(0xff0000), 0);
    lv_label_set_text(rtrnbtnLabel, "Return");
    lv_obj_center(rtrnbtnLabel);
    lv_obj_add_event_cb(rtrnbtn, goToSendRfScreen, LV_EVENT_CLICKED, NULL);
}
