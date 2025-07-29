//
// Created by salman on 24/11/24.
//
#include <string.h>
#include "../../../managed_components/lvgl__lvgl/lvgl.h"

#include <esp_log.h>
#include <ui/MainScreen.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include "IRScreen.h"
#include "Receive/ReceiveIRScreen.h"
#include "Sender/SendIrScreen.h"

static lv_obj_t *irScrn;

void goToIRScreen() {
    deletePreviousScreen(irScrn);
    ESP_LOGI("IRScreen", "Going to IR Screen");
    irScreen();
    lv_scr_load(irScrn);
}

static void irScreen() {
    irScrn = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(irScrn, lv_color_hex(0xc5d9db), 0);

    lv_obj_t *sendIR = lv_btn_create(irScrn);
    lv_obj_t *recvIR = lv_btn_create(irScrn);
    lv_obj_t *rtrn = lv_btn_create(irScrn);

    lv_obj_set_size(sendIR, 150, 50);
    lv_obj_set_pos(sendIR, 75, 30);
    lv_obj_t *sendIRlb = lv_label_create(sendIR);
    lv_label_set_text(sendIRlb, "Send IR");
    lv_obj_center(sendIRlb);
    lv_obj_add_event_cb(sendIR, goToIrSend, LV_EVENT_CLICKED, NULL);


    lv_obj_set_size(recvIR, 150, 50);
    lv_obj_set_pos(recvIR, 75, 100);
    lv_obj_t *recvIRlb = lv_label_create(recvIR);
    lv_label_set_text(recvIRlb, "Receive IR");
    lv_obj_center(recvIRlb);
    lv_obj_add_event_cb(recvIR, goToReceiveIrScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_set_size(rtrn, 150, 50);
    lv_obj_set_pos(rtrn, 75, 170);
    lv_obj_t *rtrnlb = lv_label_create(rtrn);
    lv_label_set_text(rtrnlb, "Return");
    lv_obj_center(rtrnlb);
    lv_obj_set_style_bg_color(rtrn, lv_color_hex(0xff0000), 0);
    lv_obj_add_event_cb(rtrn, goToMainScreen, LV_EVENT_CLICKED, NULL);
}
