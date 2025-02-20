//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "AlarmRfScreen.h"

#include <ui/RF/Send/SendRfScreen.h>

void alarmRfScreen() {
    alarmRfScrn = lv_obj_create(NULL);

    lv_obj_t *onBtn = lv_btn_create(alarmRfScrn);
    lv_obj_t *offBtn = lv_btn_create(alarmRfScrn);
    lv_obj_t *rtrnBtn = lv_btn_create(alarmRfScrn);

    lv_obj_t *onBtnLabel = lv_label_create(onBtn);
    lv_obj_t *offBtnLabel = lv_label_create(offBtn);
    lv_obj_t *rtrnBtnLabel = lv_label_create(rtrnBtn);

    lv_obj_set_pos(onBtn, 120, 30);
    lv_obj_set_size(onBtn, 50, 50);
    lv_obj_set_pos(offBtn, 120, 100);
    lv_obj_set_size(offBtn, 50, 50);
    lv_obj_set_pos(rtrnBtn, 70, 170);
    lv_obj_set_size(rtrnBtn, 150, 50);

    lv_label_set_text(offBtnLabel, "OFF");
    lv_label_set_text(onBtnLabel, "ON");
    lv_label_set_text(rtrnBtnLabel, "RETURN");

    lv_obj_center(onBtnLabel);
    lv_obj_center(offBtnLabel);
    lv_obj_center(rtrnBtnLabel);

    lv_obj_add_event_cb(rtrnBtn, goToSendRfScreen, LV_EVENT_CLICKED,NULL);
}
