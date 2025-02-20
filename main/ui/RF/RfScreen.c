//
// Created by salman on 24/11/24.
//
#include <string.h>
#include "RfScreen.h"

#include <esp_log.h>
#include <ui/MainScreen.h>
#include <ui/CommonUI/InterfacesUtils.h>

#include "lvgl.h"
#include "Receive/ReceiveRfScreen.h"
#include "Send/SendRfScreen.h"

static lv_obj_t *rfScrn;

void goToRfScreen(lv_event_t *event) {
    deletePreviousScreen(rfScrn);
    ESP_LOGI("RfScreen", "Going to RfScreen");
    rfScreenInit();
    lv_scr_load(rfScrn);
}


static void rfScreenInit(void) {
    // Create screen2
    rfScrn = lv_obj_create(NULL); // Create the second screen
    lv_obj_t *sendRF = lv_btn_create(rfScrn);
    lv_obj_t *recvRF = lv_btn_create(rfScrn);
    lv_obj_t *rtrn = lv_btn_create(rfScrn);

    lv_obj_set_size(sendRF, 150, 50);
    lv_obj_set_pos(sendRF, 75, 30);
    lv_obj_t *sendRFlb = lv_label_create(sendRF);
    lv_label_set_text(sendRFlb, "Send RF");
    lv_obj_center(sendRFlb);
    lv_obj_add_event_cb(sendRF, goToSendRfScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_set_size(recvRF, 150, 50);
    lv_obj_set_pos(recvRF, 75, 100);
    lv_obj_t *recvRFlb = lv_label_create(recvRF);
    lv_label_set_text(recvRFlb, "Receive RF");
    lv_obj_center(recvRFlb);
    lv_obj_add_event_cb(recvRF, goToReceiveRFscreen, LV_EVENT_CLICKED, NULL);

    lv_obj_set_size(rtrn, 150, 50);
    lv_obj_set_pos(rtrn, 75, 170);
    lv_obj_t *rtrnlb = lv_label_create(rtrn);
    lv_label_set_text(rtrnlb, "Return");
    lv_obj_center(rtrnlb);
    lv_obj_add_event_cb(rtrn, goToMainScreen, LV_EVENT_CLICKED, NULL);
}
