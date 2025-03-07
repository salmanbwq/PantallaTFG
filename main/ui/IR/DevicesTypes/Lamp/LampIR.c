//
// Created by salman on 2/03/25.
//

#include "LampIR.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <portmacro.h>
#include <string.h>
#include <ESPNOW/espNow.h>
#include <freertos/task.h>

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
static lv_obj_t *lampInstance;


void goToLampScreen(const char *instance_name, const IRActionType type) {
    if (strlen(instance_name) == 0) {
        ESP_LOGE(TAG, "No instance name provided");
        return;
    }
    strcpy(instanceName, instance_name);
    irCommandType = type;
    deletePreviousScreen(lampInstance);
    ESP_LOGI(TAG, "Going to A/C screen");
    lampScreen();
    lv_scr_load(lampInstance);
}

static void create_button(lv_obj_t *parent, lv_obj_t **btn, const char *text, lv_align_t align, lv_coord_t x_ofs,
                          lv_coord_t y_ofs, lv_color_t color, lv_coord_t width, lv_coord_t height) {
    *btn = lv_btn_create(parent);
    lv_obj_set_size(*btn, width, height);
    lv_obj_set_style_bg_color(*btn, color, 0);
    lv_obj_set_style_radius(*btn, height / 2, 0); // Botón más redondeado
    lv_obj_align(*btn, align, x_ofs, y_ofs);
    lv_obj_t *label = lv_label_create(*btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
}

static void lampScreen() {
    lampInstance = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(lampInstance, lv_color_hex(0xFFFFFF), 0); // Fondo oscuro

    // Botón de retorno
    lv_obj_t *rtrnBtn = lv_btn_create(lampInstance);
    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0xFF6363), 0);
    lv_obj_set_style_radius(rtrnBtn, 10, 0);
    lv_obj_t *rtrnLbl = lv_label_create(rtrnBtn);
    lv_label_set_text(rtrnLbl, "<-");
    lv_obj_center(rtrnLbl);
    lv_obj_align(rtrnBtn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_size(rtrnBtn, 40, 40);

    // Panel para representar la bombilla
    lv_obj_t *bulb = lv_obj_create(lampInstance);
    lv_obj_set_size(bulb, 100, 100);
    lv_obj_set_style_bg_color(bulb, lv_color_hex(0xF4E06D), 0); // Color amarillo tenue
    lv_obj_set_style_radius(bulb, 50, 0);
    lv_obj_align(bulb, LV_ALIGN_CENTER, 0, -70);

    // Control de botones
    lv_obj_t *btn_power;
    create_button(lampInstance, &btn_power, "ON/OFF", LV_ALIGN_CENTER, 0, 20, lv_color_hex(0xE84545), 80, 40);

    lv_obj_t *btn_color;
    create_button(lampInstance, &btn_color, "Color", LV_ALIGN_CENTER, 0, 80, lv_color_hex(0xF4A261), 80, 40);


    switch (irCommandType) {
        case SENDER:
            ESP_LOGI(TAG, "%s configured as sender", instanceName);
            initializeSender(instanceName, lampInstance);
            lv_obj_add_event_cb(btn_power, sendCommand, LV_EVENT_CLICKED, (char *) "Command 1");
            lv_obj_add_event_cb(btn_color, sendCommand, LV_EVENT_CLICKED, (char *) "Command 2");
            lv_obj_add_event_cb(rtrnBtn, goToIrSend, LV_EVENT_CLICKED,NULL);
            break;
        case RECEIVER:
            ESP_LOGI(TAG, "%s configured as receiver", instanceName);
            initializeReceiver(instanceName, lampInstance);
            lv_obj_add_event_cb(btn_power, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 1");
            lv_obj_add_event_cb(btn_color, receiveCommand, LV_EVENT_CLICKED, (char *) "Command 2");
            lv_obj_add_event_cb(rtrnBtn, goToReceiveIrScreen, LV_EVENT_CLICKED,NULL);
            break;
        default:
            ESP_LOGE(TAG, "Unknown command type");
    }
}
