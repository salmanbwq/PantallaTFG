//
// Created by salman on 24/02/25.
//

#include "AC.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <portmacro.h>
#include <string.h>
#include <ESPNOW/espNow.h>

#include "lvgl.h"
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/IR/DevicesTypes/Utils/COMMANDS/Receiver/IRCommandReceiver.h>
#include <ui/IR/DevicesTypes/Utils/COMMANDS/Send/IRCommandSender.h>
#include <ui/IR/Receive/ReceiveIRScreen.h>
#include <ui/IR/Sender/SendIrScreen.h>
#include <ui/IR/Utils/Enums.h>

static const char *TAG = "A/C Screen";


static char instanceName[100];
static IRActionType irCommandType;
static lv_obj_t *acInstance;


void goToACScreen(const char *instance_name, const IRActionType type) {
    if (strlen(instance_name) == 0) {
        ESP_LOGE(TAG, "No instance name provided");
        return;
    }
    strcpy(instanceName, instance_name);
    irCommandType = type;
    deletePreviousScreen(acInstance);
    ESP_LOGI(TAG, "Going to A/C screen");
    ACScreen();
    lv_scr_load(acInstance);
}

static void create_button(lv_obj_t *parent, lv_obj_t **btn, const char *text, lv_align_t align, lv_coord_t x_ofs,
                          lv_coord_t y_ofs, lv_color_t color) {
    *btn = lv_btn_create(parent);
    lv_obj_set_size(*btn, 70, 40);
    lv_obj_set_style_bg_color(*btn, color, 0);
    lv_obj_set_style_radius(*btn, 10, 0);
    lv_obj_align(*btn, align, x_ofs, y_ofs);
    lv_obj_t *label = lv_label_create(*btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
}

static void ACScreen() {
    acInstance = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(acInstance, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t *rtrnBtn = lv_btn_create(acInstance);
    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0xFF6363), 0);
    lv_obj_set_style_radius(rtrnBtn, 10, 0);
    lv_obj_t *rtrnLbl = lv_label_create(rtrnBtn);
    lv_label_set_text(rtrnLbl, "<-");
    lv_obj_center(rtrnLbl);
    lv_obj_align(rtrnBtn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_size(rtrnBtn, 40, 40);

    lv_obj_t *remote = lv_obj_create(acInstance);
    lv_obj_set_size(remote, 200, 220);
    lv_obj_clear_flag(remote, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(remote, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(remote, lv_color_hex(0xD9D9D9), 0);
    lv_obj_set_style_radius(remote, 15, 0);
    lv_obj_center(remote);

    // Botones
    lv_obj_t *btn_power;
    create_button(remote, &btn_power, LV_SYMBOL_POWER, LV_ALIGN_TOP_MID, 0, 10, lv_color_hex(0xE84545));

    lv_obj_t *btn_ok;
    create_button(remote, &btn_ok, "OK", LV_ALIGN_CENTER, 0, -5, lv_color_hex(0xA3A3A3));

    lv_obj_t *btn_temp_down;
    create_button(remote, &btn_temp_down, "TEMP-", LV_ALIGN_CENTER, -40, 60, lv_color_hex(0xA3A3A3));

    lv_obj_t *btn_temp_up;
    create_button(remote, &btn_temp_up, "TEMP+", LV_ALIGN_CENTER, 40, 60, lv_color_hex(0xA3A3A3));


    switch (irCommandType) {
        case SENDER:
            ESP_LOGI(TAG, "%s configured as sender", instanceName);
            initializeSender(instanceName, acInstance);
            lv_obj_add_event_cb(btn_ok, sendCommand, LV_EVENT_CLICKED, (char *) "Command 1");
            lv_obj_add_event_cb(btn_temp_up, sendCommand, LV_EVENT_CLICKED, (char *) "Command 2");
            lv_obj_add_event_cb(btn_power, sendCommand, LV_EVENT_CLICKED, (char *) "Command 3");
            lv_obj_add_event_cb(btn_temp_down, sendCommand, LV_EVENT_CLICKED, (char *) "Command 4");
            lv_obj_add_event_cb(rtrnBtn, goToIrSend, LV_EVENT_CLICKED,NULL);
            break;
        case RECEIVER:
            ESP_LOGI(TAG, "%s configured as receiver", instanceName);
            initializeReceiver(instanceName, acInstance);
            lv_obj_add_event_cb(btn_ok, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 1");
            lv_obj_add_event_cb(btn_temp_up, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 2");
            lv_obj_add_event_cb(btn_power, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 3");
            lv_obj_add_event_cb(btn_temp_down, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 4");
            lv_obj_add_event_cb(rtrnBtn, goToReceiveIrScreen, LV_EVENT_CLICKED,NULL);
            break;
        default:
            ESP_LOGE(TAG, "Unknown command type");
    }
}
