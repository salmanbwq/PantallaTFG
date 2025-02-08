//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "SendRfidScreen.h"

#include <esp_log.h>
#include <ui/GeneralObjects/InterfacesUtils.h>
#include <ui/IR/SendIrScreen.h>

#include "RfidScreen.h"

void goToSendRfidScreen(lv_event_t *event) {
    deletePreviousScreen(sendIrScrn);
    ESP_LOGI("SendRfidScreen", "Going to send RfidScreen");
    sendRfidScreen();
    lv_scr_load(sendRfidScrn);
}


static void sendRfidScreen(void) {
    sendRfidScrn = lv_obj_create(NULL);

    lv_obj_t *ddDisp = lv_dropdown_create(sendRfidScrn);
    lv_obj_t *ddCommand = lv_dropdown_create(sendRfidScrn);

    lv_obj_t *rtrnBtn = lv_btn_create(sendRfidScrn);
    lv_obj_t *rtrnBtnLbl = lv_label_create(rtrnBtn);

    lv_obj_t *sendBtn = lv_btn_create(sendRfidScrn);
    lv_obj_t *sendBtnLblL = lv_label_create(sendBtn);

    lv_dropdown_set_text(ddDisp, "Select dispositive");
    lv_dropdown_set_options(ddDisp, "Door\nElevator\nLocker");
    lv_obj_set_pos(ddDisp, 20, 30);
    lv_obj_set_size(ddDisp, 165, 40);

    lv_dropdown_set_text(ddCommand, "Select Command");
    lv_dropdown_set_options(ddCommand, "Open\nClose");
    lv_obj_set_pos(ddCommand, 20, 90);
    lv_obj_set_size(ddCommand, 165, 40);

    lv_label_set_text(sendBtnLblL, "Send");
    lv_obj_center(sendBtnLblL);
    lv_obj_set_pos(sendBtn, 220, 50);
    lv_obj_set_style_bg_color(sendBtn, lv_color_hex(0x00bb2d), 0);
    lv_obj_set_size(sendBtn, 70, 30);


    lv_obj_center(rtrnBtnLbl);
    lv_obj_set_size(rtrnBtn, 70, 30);
    lv_obj_set_pos(rtrnBtn, 220, 95);
    lv_label_set_text(rtrnBtnLbl, "Return");
    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0xff0000), 0);
    lv_obj_add_event_cb(rtrnBtn, goToRfidScreen, LV_EVENT_PRESSED,NULL);
}
