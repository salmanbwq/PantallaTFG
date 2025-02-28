//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "ReceiveRfidScreen.h"
#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <esp_now.h>
#include <FreeRTOSConfig.h>
#include <portmacro.h>
#include <string.h>
#include <ESPNOW/espNow.h>
#include <freertos/task.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/RFID/RfidScreen.h>
#include <ui/RFID/Utils/JSONManager/RfidDataStore.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

#define FILE_PATH "/spiffs/rfidDevices.json"
static const char *TAG = "ReceiveRfidScreen";

void goToReceiveRfidScreen(lv_event_t *event) {
    deletePreviousScreen(receiverFidScreen);
    ESP_LOGI("ReceiveRfidScreen", "Going to receive rfid");
    receiveRfidScreen();
    lv_scr_load(receiverFidScreen);
}

static void receiveRFIDData(lv_event_t *event) {
    initializeFlags();
    // Obtener el user data correctamente
    lv_obj_t **userData = lv_event_get_user_data(event);
    if (!userData) {
        ESP_LOGE(TAG, "User data is NULL");
        return;
    }
    char commandName[100];
    char dispName[100];

    // Obtener los valores de los dropdowns
    lv_dropdown_get_selected_str(userData[0], commandName, sizeof(commandName));
    lv_dropdown_get_selected_str(userData[1], dispName, sizeof(dispName));

    if (strlen(commandName) == 0 || strlen(dispName) == 0) {
        ESP_LOGI(TAG, "Received empty command or dispositive name");
        return;
    }


    char *command = "receiveRfid/";
    esp_now_send_data(lcd, (uint8_t *) command, strlen(command) + 1);

    // Esperar respuesta de ESP-NOW
    while (!hasReceived()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    // Obtener los datos recibidos
    char receivedData[100];
    strcpy(receivedData, getBuffer());
    if (strlen(receivedData) == 0) {
        ESP_LOGE(TAG, "Received null or empty data");
        showConfirmationPopup(receiverFidScreen, "Received null or empty data");
        return;
    }


    ESP_LOGI(TAG, "Received: %s to be applied in dispositive %s, command %s",
             receivedData, dispName, commandName);


    // Actualizar JSON si los datos son válidos
    if (updateRFIDJSON(dispName, commandName, receivedData) == ESP_OK) {
        showConfirmationPopup(receiverFidScreen, "Command was added");
    } else {
        showConfirmationPopup(receiverFidScreen, "Error receiving");
    }
}


static void receiveRfidScreen(void) {
    receiverFidScreen = lv_obj_create(NULL);
    lv_obj_t *ta_name = lv_dropdown_create(receiverFidScreen);
    lv_obj_t *ddDisp = lv_dropdown_create(receiverFidScreen);
    lv_obj_t *rtrnBtn = lv_btn_create(receiverFidScreen);
    lv_obj_t *rtrnBtnLbl = lv_label_create(rtrnBtn);
    lv_dropdown_clear_options(ta_name);
    lv_obj_add_state(ta_name, LV_STATE_DISABLED);


    lv_obj_t *sendBtn = lv_btn_create(receiverFidScreen);
    lv_obj_t *sendBtnLblL = lv_label_create(sendBtn);

    populateDropdownNames(ddDisp, FILE_PATH);
    lv_obj_set_pos(ddDisp, 20, 30);
    lv_obj_set_size(ddDisp, 165, 40);

    if (lv_dropdown_get_option_count(ddDisp) == 0) {
        showConfirmationPopup(receiverFidScreen, "Data empty");
        lv_obj_add_state(ddDisp, LV_STATE_DISABLED);
    }


    lv_obj_add_event_cb(ddDisp, populateDropdownCommands, LV_EVENT_ALL, ta_name);


    lv_obj_set_pos(ta_name, 20, 30);
    lv_obj_set_size(ta_name, 140, 50);
    lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 20, 105); // Posición relativa a los dropdowns

    lv_label_set_text(sendBtnLblL, "Receive");
    lv_obj_center(sendBtnLblL);
    lv_obj_set_pos(sendBtn, 220, 50);
    lv_obj_set_style_bg_color(sendBtn, lv_color_hex(0x00bb2d), 0);
    lv_obj_set_size(sendBtn, 70, 30);


    // Crear un array para almacenar las cadenas correctamente
    lv_obj_t **receiveData = malloc(2 * sizeof(lv_obj_t *));
    receiveData[0] = ta_name; // Copia segura de la cadena
    receiveData[1] = ddDisp;
    // Pasar sendData como parámetro a la función de evento
    lv_obj_add_event_cb(sendBtn, receiveRFIDData, LV_EVENT_CLICKED, receiveData);

    lv_obj_center(rtrnBtnLbl);
    lv_obj_set_size(rtrnBtn, 70, 30);
    lv_obj_set_pos(rtrnBtn, 220, 95);
    lv_label_set_text(rtrnBtnLbl, "Return");
    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0xff0000), 0);
    lv_obj_add_event_cb(rtrnBtn, goToRfidScreen, LV_EVENT_PRESSED,NULL);
}
