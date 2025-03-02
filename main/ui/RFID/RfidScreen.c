//
// Created by salman on 24/11/24.
//
#include <string.h>
#include "RfidScreen.h"

#include <esp_log.h>
#include <ESPNOW/espNow.h>
#include <ui/MainScreen.h>
#include <ui/CommonUI/InterfacesUtils.h>


#include "lvgl.h"
#include "Receive/ReceiveRfidScreen.h"
#include "Send/SendRfidScreen.h"

void goToRfidScreen(lv_event_t *event) {
    deletePreviousScreen(rfidScrn);
    ESP_LOGI("RfidScreen", "Going to RfidScreen");
    initRfidScreen();
    lv_scr_load(rfidScrn);
}

static void initRfidScreen(void) {
    // Create screen2
    rfidScrn = lv_obj_create(NULL); // Create the second screen
    lv_obj_t *sendRFID = lv_btn_create(rfidScrn);
    lv_obj_t *recvRFID = lv_btn_create(rfidScrn);
    lv_obj_t *rtrn = lv_btn_create(rfidScrn);

    lv_obj_set_size(rfidScrn, 320, 240); // Tamaño de la pantalla
    lv_obj_set_style_bg_color(rfidScrn, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(rfidScrn, LV_OPA_COVER, 0);

    lv_obj_set_size(sendRFID, 150, 50);
    lv_obj_set_pos(sendRFID, 75, 30);
    lv_obj_t *sendRFIDlb = lv_label_create(sendRFID);
    lv_label_set_text(sendRFIDlb, "Send RFID");
    lv_obj_center(sendRFIDlb);
    lv_obj_add_event_cb(sendRFID, goToSendRfidScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_set_size(recvRFID, 150, 50);
    lv_obj_set_pos(recvRFID, 75, 100);
    lv_obj_t *recvRFIDlb = lv_label_create(recvRFID);
    lv_label_set_text(recvRFIDlb, "Receive RFID");
    lv_obj_center(recvRFIDlb);
    lv_obj_add_event_cb(recvRFID, goToReceiveRfidScreen, LV_EVENT_CLICKED, NULL);


    lv_obj_set_size(rtrn, 150, 50);
    lv_obj_set_pos(rtrn, 75, 170);
    lv_obj_t *rtrnlb = lv_label_create(rtrn);
    lv_label_set_text(rtrnlb, "Return");
    lv_obj_center(rtrnlb);
    lv_obj_set_style_bg_color(rtrn, lv_color_hex(0xff0000), 0);
    lv_obj_add_event_cb(rtrn, goToMainScreen, LV_EVENT_CLICKED, NULL);
}
