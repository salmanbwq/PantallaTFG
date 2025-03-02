//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "TVIRScreen.h"

#include <freertos/FreeRTOS.h>
#include <portmacro.h>

#include <esp_log.h>
#include <string.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/IR/DevicesTypes/Utils/COMMANDS/Receiver/IRCommandReceiver.h>
#include <ui/IR/DevicesTypes/Utils/COMMANDS/Send/IRCommandSender.h>
#include <ui/IR/Receive/ReceiveIRScreen.h>
#include <ui/IR/Sender/SendIrScreen.h>
#include <ui/IR/Utils/Enums.h>

static const char *TAG = "Tv Ir Screen";
static lv_obj_t *tvIRInstance;
static char instanceName[100];
static IRActionType irCommandType;

void goToTvIRScreen(const char *instance_name, const IRActionType type) {
    if (strlen(instance_name) == 0) {
        ESP_LOGE(TAG, "No instance name provided");
        return;
    }
    strcpy(instanceName, instance_name);
    irCommandType = type;
    deletePreviousScreen(tvIRInstance);
    ESP_LOGI(TAG, "Going to TV IR screen");
    tvIRScreen();
    lv_scr_load(tvIRInstance);
}

static void createVolButton(lv_obj_t *parent, lv_obj_t **btn, const char *text, lv_align_t align, lv_coord_t x_ofs,
                            lv_coord_t y_ofs, lv_color_t color) {
    *btn = lv_btn_create(parent);
    lv_obj_set_size(*btn, 40, 50);
    lv_obj_set_style_bg_color(*btn, color, 0);
    lv_obj_set_style_radius(*btn, 5, 0);
    lv_obj_align(*btn, align, x_ofs, y_ofs);
    lv_obj_t *label = lv_label_create(*btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
}

static void createONOFFOKButton(lv_obj_t *parent, lv_obj_t **btn, const char *text, lv_align_t align, lv_coord_t x_ofs,
                                lv_coord_t y_ofs, lv_color_t color) {
    *btn = lv_btn_create(parent);
    lv_obj_set_size(*btn, 35, 35);
    lv_obj_set_style_bg_color(*btn, color, 0);
    lv_obj_set_style_radius(*btn, 100, 0);
    lv_obj_align(*btn, align, x_ofs, y_ofs);
    lv_obj_t *label = lv_label_create(*btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
}

static void tvIRScreen() {
    tvIRInstance = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(tvIRInstance, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t *rtrnBtn = lv_btn_create(tvIRInstance);
    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0xFF6363), 0);
    lv_obj_set_style_radius(rtrnBtn, 10, 0);
    lv_obj_t *rtrnLbl = lv_label_create(rtrnBtn);
    lv_label_set_text(rtrnLbl, "<-");
    lv_obj_center(rtrnLbl);
    lv_obj_align(rtrnBtn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_size(rtrnBtn, 40, 40);

    lv_obj_t *remote = lv_obj_create(tvIRInstance);
    lv_obj_set_size(remote, 200, 260);
    lv_obj_align(remote, LV_ALIGN_CENTER, 0, -50);
    lv_obj_add_flag(rtrnBtn, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_style_bg_color(remote, lv_color_hex(0x222831), 0);
    lv_obj_set_style_radius(remote, 15, 0);
    lv_obj_center(remote);


    // Botón de encendido
    lv_obj_t *btn_power;
    createONOFFOKButton(remote, &btn_power, LV_SYMBOL_POWER, LV_ALIGN_TOP_LEFT, 0, 0, lv_color_hex(0xE84545));

    // Botón de volumen +
    lv_obj_t *btn_vol_up;
    createVolButton(remote, &btn_vol_up, LV_SYMBOL_VOLUME_MAX, LV_ALIGN_LEFT_MID, 0, -40, lv_color_hex(0x0B5394));

    // Botón de volumen -
    lv_obj_t *btn_vol_down;
    createVolButton(remote, &btn_vol_down, LV_SYMBOL_VOLUME_MID, LV_ALIGN_LEFT_MID, 0, 20, lv_color_hex(0x0B5394));

    // Botón de canal +
    lv_obj_t *btn_ch_up;
    createVolButton(remote, &btn_ch_up, "CH\n+", LV_ALIGN_RIGHT_MID, 0, -40, lv_color_hex(0x00917C));

    // Botón de canal -
    lv_obj_t *btn_ch_down;
    createVolButton(remote, &btn_ch_down, "CH\n-", LV_ALIGN_RIGHT_MID, 0, 20, lv_color_hex(0x00917C));

    // Botón OK en el centro
    lv_obj_t *btn_ok;
    createONOFFOKButton(remote, &btn_ok, "OK", LV_ALIGN_CENTER, 0, 70, lv_color_hex(0xF4A261));

    switch (irCommandType) {
        case SENDER:
            ESP_LOGI(TAG, "%s configured as sender", instanceName);
            initializeSender(instanceName, tvIRInstance);
            lv_obj_add_event_cb(btn_ok, sendCommand, LV_EVENT_CLICKED, (char *) "Command 1");
            lv_obj_add_event_cb(btn_ch_down, sendCommand, LV_EVENT_CLICKED, (char *) "Command 2");
            lv_obj_add_event_cb(btn_ch_up, sendCommand, LV_EVENT_CLICKED, (char *) "Command 3");
            lv_obj_add_event_cb(btn_vol_up, sendCommand, LV_EVENT_CLICKED, (char *) "Command 4");
            lv_obj_add_event_cb(btn_power, sendCommand, LV_EVENT_CLICKED, (char *) "Command 5");
            lv_obj_add_event_cb(btn_vol_down, sendCommand, LV_EVENT_CLICKED, (char *) "Command 6");
            lv_obj_add_event_cb(rtrnBtn, goToIrSend, LV_EVENT_CLICKED,NULL);
            break;
        case RECEIVER:
            ESP_LOGI(TAG, "%s configured as receiver", instanceName);
            initializeReceiver(instanceName, tvIRInstance);
            lv_obj_add_event_cb(btn_ok, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 1");
            lv_obj_add_event_cb(btn_ch_down, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 2");
            lv_obj_add_event_cb(btn_ch_up, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 3");
            lv_obj_add_event_cb(btn_vol_up, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 4");
            lv_obj_add_event_cb(btn_power, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 5");
            lv_obj_add_event_cb(btn_vol_down, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 6");
            lv_obj_add_event_cb(rtrnBtn, goToReceiveIrScreen, LV_EVENT_CLICKED,NULL);
            break;
        default:
            ESP_LOGE(TAG, "Unknown command type");
    }
}
