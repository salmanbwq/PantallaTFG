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
#include <ui/RFID/RfidScreen.h>
#include <ui/RFID/Utils/JSONManager/RFIDDataRead.h>
#include <ui/RFID/Utils/JSONManager/RfidDataStore.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

static char *FILE_PATH = "/spiffs/rfidDevices.json";
static char *TAG = "SendRfidScreen";

static lv_obj_t *sendRfidScrn;
static lv_obj_t *ddDisp;
static lv_obj_t *textEntrance;
static lv_obj_t *textEntranceLabel;
static lv_obj_t *dropdownLabel;
static lv_obj_t *checkBox;
static lv_obj_t *ddCommand;
static lv_obj_t *keyboard;
static lv_obj_t *labelOpt1;
static lv_obj_t *labelOpt2;

void goToSendRfidScreen() {
    deletePreviousScreen(sendRfidScrn);
    ESP_LOGI("SendRfidScreen", "Going to send RfidScreen");
    sendRfidScreen();
    lv_scr_load(sendRfidScrn);
}

static void textAreaHandlerRFID(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_FOCUSED) {
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Mostrar teclado
        lv_keyboard_set_textarea(keyboard, textEntrance);

        lv_obj_add_flag(checkBox, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(labelOpt1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(labelOpt2, LV_OBJ_FLAG_HIDDEN);
    } else if (code == LV_EVENT_DEFOCUSED) {
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Ocultar teclado
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_clear_flag(checkBox, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(labelOpt1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(labelOpt2, LV_OBJ_FLAG_HIDDEN);
    }
}


static void createKeyboardRFID(lv_obj_t *parent) {
    keyboard = lv_keyboard_create(parent);
    lv_obj_set_size(keyboard, 320, 100);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

static void onSwitchChange(lv_event_t *event) {
    lv_obj_t *switchObj = lv_event_get_target(event);

    // Verificar si los objetos existen antes de usarlos
    if (!checkBox || !ddCommand || !ddDisp || !textEntrance) {
        ESP_LOGE(TAG, "One or more UI components are NULL");
        return;
    }

    if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED) {
        if (lv_obj_has_state(switchObj, LV_STATE_CHECKED)) {
            lv_obj_add_flag(ddCommand, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ddDisp, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(dropdownLabel, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(textEntrance, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(textEntranceLabel, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_clear_flag(ddCommand, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ddDisp, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(dropdownLabel, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(textEntrance, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(textEntranceLabel, LV_OBJ_FLAG_HIDDEN);
        }
    }
}


static void onRFIDSendAction() {
    //ESP_LOGI(TAG, "New send action triggered!");
    // Aquí puedes agregar la nueva funcionalidad deseada
    if (lv_obj_has_state(checkBox, LV_STATE_CHECKED)) {
        if (strcmp(lv_textarea_get_text(textEntrance), "") == 0) {
            ESP_LOGI(TAG, "Command to send is empty");
            showConfirmationPopup(sendRfidScrn, "Command to send is empty");
            return;
        };
        if (strlen(lv_textarea_get_text(textEntrance)) > 14) {
            ESP_LOGI(TAG, "Command to send is too long");
            showConfirmationPopup(sendRfidScrn, "Command to long");
            return;
        }
        char command[20];
        strcpy(command, lv_textarea_get_text(textEntrance));
        char commandToSend[150];
        sprintf(commandToSend, "sendRfid/%s/", command);
        sendEspNowSendCommandRFID(commandToSend);
        lv_textarea_set_text(textEntrance, "");
    } else {
        sendStoredCommadRFID();
    }
}


static void sendEspNowSendCommandRFID(char commandToSend[150]) {
    initializeFlags();

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

static void sendStoredCommadRFID() {
    char commandName[100];
    char dispName[100];
    lv_dropdown_get_selected_str(ddCommand, commandName, sizeof(commandName));
    lv_dropdown_get_selected_str(ddDisp, dispName, sizeof(dispName));


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

    sendEspNowSendCommandRFID(commandToSend);
}


static void sendRfidScreen(void) {
    sendRfidScrn = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(sendRfidScrn, lv_color_hex(0xc5d9db), 0);

    ddDisp = lv_dropdown_create(sendRfidScrn);
    ddCommand = lv_dropdown_create(sendRfidScrn);

    lv_obj_t *rtrnBtn = lv_btn_create(sendRfidScrn);
    lv_obj_t *rtrnBtnLbl = lv_label_create(rtrnBtn);

    lv_dropdown_clear_options(ddCommand);
    lv_obj_add_state(ddCommand, LV_STATE_DISABLED);

    dropdownLabel = lv_label_create(sendRfidScrn);
    lv_label_set_text(dropdownLabel, "Select Dispositive");
    lv_obj_align(dropdownLabel, LV_ALIGN_CENTER, -80, -100);


    textEntranceLabel = lv_label_create(sendRfidScrn);
    lv_label_set_text(textEntranceLabel, "Enter Command");
    lv_obj_align(textEntranceLabel, LV_ALIGN_CENTER, -80, -100);
    lv_obj_add_flag(textEntranceLabel, LV_OBJ_FLAG_HIDDEN);
    createKeyboardRFID(sendRfidScrn);


    textEntrance = lv_textarea_create(sendRfidScrn);
    lv_obj_align(textEntrance, LV_ALIGN_CENTER, -60, -60);
    lv_obj_set_size(textEntrance, 165, 40);
    lv_textarea_set_placeholder_text(textEntrance, "Command");
    lv_obj_add_flag(textEntrance, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(textEntrance, textAreaHandlerRFID, LV_EVENT_ALL, NULL);


    lv_obj_align(ddCommand, LV_ALIGN_CENTER, -60, 0);
    lv_obj_set_size(ddCommand, 165, 40);


    lv_obj_t *sendBtn = lv_btn_create(sendRfidScrn);
    lv_obj_t *sendBtnLblL = lv_label_create(sendBtn);

    populateDropdownNames(ddDisp, FILE_PATH);
    lv_obj_align(ddDisp, LV_ALIGN_CENTER, -60, -60);
    lv_obj_set_size(ddDisp, 165, 40);
    lv_obj_set_size(ddDisp, 165, 40);

    if (lv_dropdown_get_option_count(ddDisp) == 0) {
        showConfirmationPopup(sendRfidScrn, "Data empty");
        lv_obj_add_state(ddDisp, LV_STATE_DISABLED);
    }

    lv_obj_add_event_cb(ddDisp, populateDropdownCommands, LV_EVENT_ALL, ddCommand);


    labelOpt1 = lv_label_create(sendRfidScrn);
    lv_label_set_text(labelOpt1, "Search\ncommand");
    lv_obj_align(labelOpt1, LV_ALIGN_LEFT_MID, 20, 60);

    labelOpt2 = lv_label_create(sendRfidScrn);
    lv_label_set_text(labelOpt2, "Write\ncommand");
    lv_obj_align(labelOpt2, LV_ALIGN_RIGHT_MID, -70, 60);


    lv_label_set_text(sendBtnLblL, "Send");
    lv_obj_center(sendBtnLblL);
    lv_obj_align(sendBtn, LV_ALIGN_CENTER, 90, -60);
    lv_obj_set_style_bg_color(sendBtn, lv_color_hex(0x00bb2d), 0);
    lv_obj_set_size(sendBtn, 70, 30);


    lv_obj_add_event_cb(sendBtn, onRFIDSendAction, LV_EVENT_PRESSED, NULL);


    lv_obj_center(rtrnBtnLbl);
    lv_obj_set_size(rtrnBtn, 70, 30);
    lv_obj_align(rtrnBtn, LV_ALIGN_CENTER, 90, -20);
    lv_label_set_text(rtrnBtnLbl, "Return");
    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0xff0000), 0);
    lv_obj_add_event_cb(rtrnBtn, goToRfidScreen, LV_EVENT_PRESSED,NULL);

    checkBox = lv_switch_create(sendRfidScrn);
    lv_obj_align(checkBox, LV_ALIGN_CENTER, -25, 60);
    lv_obj_add_event_cb(checkBox, onSwitchChange, LV_EVENT_ALL, NULL);
}
