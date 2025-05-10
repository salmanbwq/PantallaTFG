//
// Created by salman on 7/03/25.
//

#include "RFCommandReceiver.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <portmacro.h>
#include <string.h>
#include "lvgl.h"
#include <ESPNOW/espNow.h>
#include <freertos/task.h>
#include <ui/RF/Utils/JSONManager/RFDataRead.h>
#include <ui/RF/Utils/JSONManager/RFDataStore.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>


static char instanceName[100];
static lv_obj_t *receiverRFInstance;

static char *TAG = "Receive RF command";

void initializeRFReceiver(char instance[100], lv_obj_t *instanceObj) {
    if (instanceObj == NULL || strlen(instance) == 0) {
        ESP_LOGE(TAG, "Sender initialization failed");
        return;
    }

    strcpy(instanceName, instance);
    receiverRFInstance = instanceObj;
    ESP_LOGI(TAG, "Receiver initialized with %s info", instanceName);
}

void receiveRFCommand(const char *commandName) {
    initializeFlags();
    if (strlen(commandName) == 0) {
        ESP_LOGI(TAG, "Command not found");
        return;
    }

    char *aux = getDeviceType(instanceName);

    if (!aux || strlen(aux) == 0) {
        ESP_LOGI(TAG, "Command not found");
        return;
    }

    char commandToSend[200];
    char type[20];

    strcpy(type, aux);

    sprintf(commandToSend, "receiveRF/%s/%s", type, commandName);

    esp_now_send_data(lcd, (uint8_t *) commandToSend, sizeof(commandToSend));

    TickType_t startTime = xTaskGetTickCount();

    while (!hasReceived()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        if ((xTaskGetTickCount() - startTime) * portTICK_PERIOD_MS > 5000) {
            ESP_LOGE(TAG, "Timeout: receiveRFID() no respondió en 5 segundos");
            showConfirmationPopup(receiverRFInstance, "Timeout");
            return;
        }
    }

    if (getResult() == ESP_OK) {
        ESP_LOGI(TAG, "Received command for %s", commandName);
    } else {
        showConfirmationPopup(receiverRFInstance, "Failed receiving");
        ESP_LOGI(TAG, "Failed receiving command %s", commandName);
        return;
    }

    char commandReceived[50];
    strcpy(commandReceived, getBuffer());

    if (updateRFJSON(instanceName, commandName, commandReceived) == ESP_OK) {
        showConfirmationPopup(receiverRFInstance, "Command stored");
    } else {
        //update command
        showConfirmationPopup(receiverRFInstance, "Error storing command");
        ESP_LOGE(TAG, "Failed storing command");
    }
}
