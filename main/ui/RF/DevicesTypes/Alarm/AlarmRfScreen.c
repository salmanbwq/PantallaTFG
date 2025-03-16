//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "AlarmRfScreen.h"

#include <esp_log.h>
#include <string.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/RF/Receive/ReceiveRfScreen.h>
#include <ui/RF/Send/SendRfScreen.h>
#include <ui/RF/Utils/Enums.h>
#include <ui/RF/Utils/Commands/Receiver/RFCommandReceiver.h>
#include <ui/RF/Utils/Commands/Sender/RFCommandSender.h>
static lv_obj_t *alarmRfScrn;
static const char *TAG = "Alarm RF Screen";
static char instanceName[100];

typedef enum Command {
    ON, OFF
} AlarmCommand;


void loadAlarmScreen(const char *name, const DispositiveSelectorType type) {
    if (!alarmRfScrn) {
        alarmRfScrn = lv_obj_create(NULL);
    }
    ESP_LOGI(TAG, "Going to Alarm Screen");
    deletePreviousScreen(alarmRfScrn);
    alarmRfScreen(name, type);
    lv_scr_load(alarmRfScrn);
}

static void alarmSendCommandRF(lv_event_t *event) {
    const AlarmCommand command = *(AlarmCommand *) lv_event_get_user_data(event);
    switch (command) {
        case ON:
            ESP_LOGI(TAG, "Sending Open Command");
        sendRFCommand("ON");
        break;
        case OFF:
            ESP_LOGI(TAG, "Sending Close Command");
        sendRFCommand("OFF");
        break;
    }
}

static void alarmReceiveCommandRF(lv_event_t *event) {
    const AlarmCommand command = *(AlarmCommand *) lv_event_get_user_data(event);
    switch (command) {
        case ON:
            ESP_LOGI(TAG, "Receiving Open Command");
        receiveRFCommand("ON");
        break;
        case OFF:
            ESP_LOGI(TAG, "Receiving Close Command");
        receiveRFCommand("OFF");
        break;
    }
}


static void alarmRfScreen(const char *name, const DispositiveSelectorType type) {
    alarmRfScrn = lv_obj_create(NULL);
    strcpy(instanceName, name);
    static AlarmCommand open = ON;
    static AlarmCommand close = OFF;

    lv_obj_set_style_bg_color(alarmRfScrn, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_radius(alarmRfScrn, 10, LV_PART_MAIN);

    /* Título de la pantalla */
    lv_obj_t *title = lv_label_create(alarmRfScrn);
    lv_label_set_text(title, "ALARM CONTROL");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Botón ON */
    lv_obj_t *onBtn = lv_btn_create(alarmRfScrn);
    lv_obj_set_size(onBtn, 100, 50);
    lv_obj_set_style_bg_color(onBtn, lv_color_hex(0x00AA00), LV_PART_MAIN);
    lv_obj_set_style_radius(onBtn, 10, LV_PART_MAIN);
    lv_obj_align(onBtn, LV_ALIGN_CENTER, -60, -40);

    lv_obj_t *onBtnLabel = lv_label_create(onBtn);
    lv_label_set_text(onBtnLabel, "ON 🔔");
    lv_obj_set_style_text_color(onBtnLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(onBtnLabel);

    /* Botón OFF */
    lv_obj_t *offBtn = lv_btn_create(alarmRfScrn);
    lv_obj_set_size(offBtn, 100, 50);
    lv_obj_set_style_bg_color(offBtn, lv_color_hex(0xAA0000), LV_PART_MAIN);
    lv_obj_set_style_radius(offBtn, 10, LV_PART_MAIN);
    lv_obj_align(offBtn, LV_ALIGN_CENTER, 60, -40);

    lv_obj_t *offBtnLabel = lv_label_create(offBtn);
    lv_label_set_text(offBtnLabel, "OFF");
    lv_obj_set_style_text_color(offBtnLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(offBtnLabel);

    /* Botón RETURN */
    lv_obj_t *rtrnBtn = lv_btn_create(alarmRfScrn);
    lv_obj_set_size(rtrnBtn, 180, 50);
    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0x4444AA), LV_PART_MAIN);
    lv_obj_set_style_radius(rtrnBtn, 10, LV_PART_MAIN);
    lv_obj_align(rtrnBtn, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t *rtrnBtnLabel = lv_label_create(rtrnBtn);
    lv_label_set_text(rtrnBtnLabel, "RETURN ⬅️");
    lv_obj_set_style_text_color(rtrnBtnLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(rtrnBtnLabel);

    /* Evento del botón RETURN */
    lv_obj_add_event_cb(rtrnBtn, goToSendRfScreen, LV_EVENT_CLICKED, NULL);

    switch (type) {
        case SENDER:
            ESP_LOGI(TAG, "SENDER");
        // Change callbacks to sender
        initializeRFSender(instanceName, alarmRfScrn);
        lv_obj_add_event_cb(onBtn, alarmSendCommandRF, LV_EVENT_CLICKED, &open);
        lv_obj_add_event_cb(offBtn, alarmSendCommandRF, LV_EVENT_CLICKED, &close);
        lv_obj_add_event_cb(rtrnBtn, goToSendRfScreen, LV_EVENT_CLICKED, NULL);
        break;
        case RECEIVER:
            // Changer callbacks to receiver
                ESP_LOGI(TAG, "RECEIVER");
        initializeRFReceiver(instanceName, alarmRfScrn);
        lv_obj_add_event_cb(onBtn, alarmReceiveCommandRF, LV_EVENT_CLICKED, &open);
        lv_obj_add_event_cb(offBtn, alarmReceiveCommandRF, LV_EVENT_CLICKED, &close);
        lv_obj_add_event_cb(rtrnBtn, goToReceiveRFscreen, LV_EVENT_CLICKED, NULL);
        break;
        default:
            ESP_LOGI(TAG, "UNKNOWN TYPE");
    }
}
