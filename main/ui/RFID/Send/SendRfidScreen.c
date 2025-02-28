//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "SendRfidScreen.h"
#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <FreeRTOSConfig.h>
#include <portmacro.h>
#include <string.h>
#include <ESPNOW/espNow.h>
#include <freertos/task.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/IR/Sender/SendIrScreen.h>
#include <ui/RFID/RfidScreen.h>
#include <ui/RFID/Utils/JSONManager/RFIDDataRead.h>
#include <ui/RFID/Utils/JSONManager/RfidDataStore.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

static char *FILE_PATH = "/spiffs/rfidDevices.json";
static char *TAG = "SendRfidScreen";

void goToSendRfidScreen(lv_event_t *event) {
    deletePreviousScreen(sendRfidScrn);
    ESP_LOGI("SendRfidScreen", "Going to send RfidScreen");
    sendRfidScreen();
    lv_scr_load(sendRfidScrn);
}


static void sendCommadRFID(lv_event_t *event) {
    initializeFlags();
    // Obtener el user data correctamente
    lv_obj_t **sendData = lv_event_get_user_data(event);
    if (!sendData) {
        ESP_LOGE(TAG, "User data is NULL");
        return;
    }


    char commandName[100];
    char dispName[100];
    lv_dropdown_get_selected_str(sendData[1], commandName, sizeof(commandName));
    lv_dropdown_get_selected_str(sendData[0], dispName, sizeof(dispName));


    // Verificar si dispName y commandName no están vacíos
    ESP_LOGI(TAG, "Received command: %s and dispositive %s", commandName, dispName);

    if (strlen(commandName) == 0 || strlen(dispName) == 0) {
        ESP_LOGE(TAG, "Command is empty");
        return;
    }

    char *command = getCommandByName(dispName, commandName);
    if (strlen(command) == 0) {
        ESP_LOGE(TAG, "Command is empty");
        return;
    }

    char commandToSend[150];
    sprintf(commandToSend, "sendRfid/%s/", command);

    esp_now_send_data(lcd, (uint8_t *) commandToSend, strlen(commandToSend) + 1);

    while (!hasSent()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    if (getResult() == ESP_OK) {
        showConfirmationPopup(sendRfidScrn, "Command was sent");
    } else {
        showConfirmationPopup(sendRfidScrn, "Error sending command");
        ESP_LOGE(TAG, "Command was not sent");
    }
}


static void sendRfidScreen(void) {
    sendRfidScrn = lv_obj_create(NULL);

    lv_obj_t *ddDisp = lv_dropdown_create(sendRfidScrn);
    lv_obj_t *ddCommand = lv_dropdown_create(sendRfidScrn);

    lv_obj_t *rtrnBtn = lv_btn_create(sendRfidScrn);
    lv_obj_t *rtrnBtnLbl = lv_label_create(rtrnBtn);

    lv_obj_t *sendBtn = lv_btn_create(sendRfidScrn);
    lv_obj_t *sendBtnLblL = lv_label_create(sendBtn);
    lv_dropdown_clear_options(ddCommand);
    lv_obj_add_state(ddCommand, LV_STATE_DISABLED);

    populateDropdownNames(ddDisp, FILE_PATH);
    lv_obj_set_pos(ddDisp, 20, 30);
    lv_obj_set_size(ddDisp, 165, 40);

    if (lv_dropdown_get_option_count(ddDisp) == 0) {
        showConfirmationPopup(sendRfidScrn, "Data empty");
        lv_obj_add_state(ddDisp, LV_STATE_DISABLED);
    }

    lv_obj_add_event_cb(ddDisp, populateDropdownCommands, LV_EVENT_ALL, ddCommand);

    lv_obj_set_pos(ddCommand, 20, 90);
    lv_obj_set_size(ddCommand, 165, 40);

    lv_label_set_text(sendBtnLblL, "Send");
    lv_obj_center(sendBtnLblL);
    lv_obj_set_pos(sendBtn, 220, 50);
    lv_obj_set_style_bg_color(sendBtn, lv_color_hex(0x00bb2d), 0);
    lv_obj_set_size(sendBtn, 70, 30);


    lv_obj_t **sendData = malloc(2 * sizeof(char *));
    sendData[0] = ddDisp; // Copia segura de la cadena
    sendData[1] = ddCommand;


    lv_obj_add_event_cb(sendBtn, sendCommadRFID, LV_EVENT_PRESSED, sendData);


    lv_obj_center(rtrnBtnLbl);
    lv_obj_set_size(rtrnBtn, 70, 30);
    lv_obj_set_pos(rtrnBtn, 220, 95);
    lv_label_set_text(rtrnBtnLbl, "Return");
    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0xff0000), 0);
    lv_obj_add_event_cb(rtrnBtn, goToRfidScreen, LV_EVENT_PRESSED,NULL);
}
