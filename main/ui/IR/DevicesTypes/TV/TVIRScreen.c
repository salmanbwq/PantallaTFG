//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "TVIRScreen.h"

#include <freertos/FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <portmacro.h>

#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <ESPNOW/espNow.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/IR/Receive/ReceiveIRScreen.h>
#include <ui/IR/Sender/SendIrScreen.h>
#include <ui/IR/Utils/Enums.h>
#include <ui/IR/Utils/JSONManager/IRDataRead.h>
#include <ui/IR/Utils/JSONManager/IRDataStore.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

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

static void sendCommand(lv_event_t *e) {
    initializeFlags();
    const lv_obj_t *btn = lv_event_get_target(e);
    const char *txt = lv_label_get_text(lv_obj_get_child(btn, 0));
    const char commandName[100];
    strcpy(commandName, lv_event_get_user_data(e));
    if (txt == NULL) {
        return;
    }
    ESP_LOGI(TAG, "Button %s pressed\n", txt);

    char buffer[10];
    strcpy(buffer, getIRCommand(instanceName, commandName));


    ESP_LOGI(TAG, "Sending IR command: %s", buffer);

    if (strlen(buffer) <= 0) {
        ESP_LOGE(TAG, "No command was received");
        return;
    }

    char cmd[100];
    snprintf(cmd, sizeof(cmd), "sendIr/%s/", buffer); // Protege contra desbordamiento

    esp_now_send_data(lcd, (uint8_t *) cmd, strlen(cmd) + 1); // Incluye '\0' final


    while (!hasSent()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    if (getResult() == ESP_OK) {
        showConfirmationPopup(tvIRInstance, "Command was sent");
    } else {
        showConfirmationPopup(tvIRInstance, "Error sending command");
        ESP_LOGE(TAG, "Command was not sent");
    }
}


static void receiveCommand(lv_event_t *e) {
    initializeFlags();
    const char txt[100];
    strcpy(txt, lv_event_get_user_data(e));
    if (strlen(txt) <= 0) {
        return;
    }
    ESP_LOGI(TAG, "Button %s pressed\n", txt);

    char cmd[100];
    strcpy(cmd, "receiveIR/");

    esp_now_send_data(lcd, (uint8_t *) cmd, strlen(cmd));
    int cont = 0;
    while (!hasReceived()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    ESP_LOGI(TAG, "Received IR command");

    if (getResult() == ESP_OK) {
        char buffer[10];
        strcpy(buffer, getBuffer());
        if (strlen(buffer) <= 0) {
            ESP_LOGE(TAG, "No command was received");
            showConfirmationPopup(tvIRInstance, "Command empty");
            return;
        }
        updateIRJSON(instanceName, txt, buffer);
        showConfirmationPopup(tvIRInstance, "Command was sent");
    } else {
        showConfirmationPopup(tvIRInstance, "Error receiving command");
        ESP_LOGE(TAG, "Command was not sent");
    }
}


static void tvIRScreen() {
    tvIRInstance = lv_obj_create(NULL);

    lv_obj_t *rtrnBtn = lv_btn_create(tvIRInstance);
    lv_obj_t *rtrnLbl = lv_label_create(rtrnBtn);
    lv_obj_t *remote = lv_obj_create(tvIRInstance);
    lv_obj_set_size(remote, 200, 220);
    lv_obj_clear_flag(remote, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(remote, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_center(remote);

    // Botón de encendido
    lv_obj_t *btn_power = lv_btn_create(remote);
    lv_obj_set_size(btn_power, 70, 30);
    lv_obj_align(btn_power, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_t *label_power = lv_label_create(btn_power);
    lv_label_set_text(label_power, "ON/OFF");
    lv_obj_center(label_power);

    // Botón de volumen +
    lv_obj_t *btn_vol_up = lv_btn_create(remote);
    lv_obj_set_size(btn_vol_up, 50, 30);
    lv_obj_align(btn_vol_up, LV_ALIGN_LEFT_MID, 0, -30);
    lv_obj_t *label_vol_up = lv_label_create(btn_vol_up);
    lv_label_set_text(label_vol_up, "+");
    lv_obj_center(label_vol_up);

    // Botón de volumen -
    lv_obj_t *btn_vol_down = lv_btn_create(remote);
    lv_obj_set_size(btn_vol_down, 50, 30);
    lv_obj_align(btn_vol_down, LV_ALIGN_LEFT_MID, 0, 20);
    lv_obj_t *label_vol_down = lv_label_create(btn_vol_down);
    lv_label_set_text(label_vol_down, "-");
    lv_obj_center(label_vol_down);

    // Botón de canal +
    lv_obj_t *btn_ch_up = lv_btn_create(remote);
    lv_obj_set_size(btn_ch_up, 50, 30);
    lv_obj_align(btn_ch_up, LV_ALIGN_RIGHT_MID, 0, -30);
    lv_obj_t *label_ch_up = lv_label_create(btn_ch_up);
    lv_label_set_text(label_ch_up, "+");
    lv_obj_center(label_ch_up);

    // Botón de canal -
    lv_obj_t *btn_ch_down = lv_btn_create(remote);
    lv_obj_set_size(btn_ch_down, 50, 30);
    lv_obj_align(btn_ch_down, LV_ALIGN_RIGHT_MID, 0, 20);
    lv_obj_t *label_ch_down = lv_label_create(btn_ch_down);
    lv_label_set_text(label_ch_down, "-");
    lv_obj_center(label_ch_down);

    // Botón OK en el centro
    lv_obj_t *btn_ok = lv_btn_create(remote);
    lv_obj_set_size(btn_ok, 60, 40);
    lv_obj_align(btn_ok, LV_ALIGN_CENTER, 0, 80);
    lv_obj_t *label_ok = lv_label_create(btn_ok);
    lv_label_set_text(label_ok, "OK");
    lv_obj_center(label_ok);

    lv_obj_align(rtrnBtn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_size(rtrnBtn, 40, 40);
    lv_label_set_text(rtrnLbl, "<-");
    lv_obj_center(rtrnLbl);

    switch (irCommandType) {
        case SENDER:
            lv_obj_add_event_cb(btn_ok, sendCommand, LV_EVENT_CLICKED, (char *) "Command 1");
            lv_obj_add_event_cb(btn_ch_down, sendCommand, LV_EVENT_CLICKED, (char *) "Command 2");
            lv_obj_add_event_cb(btn_ch_up, sendCommand, LV_EVENT_CLICKED, (char *) "Command 3");
            lv_obj_add_event_cb(btn_vol_up, sendCommand, LV_EVENT_CLICKED, (char *) "Command 4");
            lv_obj_add_event_cb(btn_power, sendCommand, LV_EVENT_CLICKED, (char *) "Command 5");
            lv_obj_add_event_cb(btn_vol_down, sendCommand, LV_EVENT_CLICKED, (char *) "Command 6");
            lv_obj_add_event_cb(rtrnBtn, goToIrSend, LV_EVENT_CLICKED,NULL);
            break;
        case RECEIVER:
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
