//
// Created by salman on 7/03/25.
//

#include "RFCommandSender.h"

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
static lv_obj_t *senderRFInstance;

static char *TAG = "Sender RF command";

void initializeRFSender(char instance[100], lv_obj_t *instanceObj) {
    if (instanceObj == NULL || strlen(instance) == 0) {
        ESP_LOGE(TAG, "Sender initialization failed");
        return;
    }

    strcpy(instanceName, instance);
    senderRFInstance = instanceObj;
    ESP_LOGI(TAG, "Sender initialized with %s info", instanceName);
}


void sendRFCommand(const char *commandName) {
    if (!commandName || strlen(commandName) == 0) {
        ESP_LOGI(TAG, "Command name is empty");
        return;
    }

    char commandStr[100];
    const char *commandFromJSON = getCommandsFromJSON(instanceName, commandName);
    const char *aux = getDeviceType(instanceName);

    if (!aux || strlen(aux) == 0) {
        ESP_LOGI(TAG, "Command not found");
        return;
    }

    if (!commandFromJSON || strlen(commandFromJSON) == 0) {
        showConfirmationPopup(senderRFInstance, "Command is empty");
        ESP_LOGI(TAG, "Command to send is empty");
        return;
    }
    char type[20];
    strcpy(type, aux);
    strcpy(commandStr, commandFromJSON);

    char commandToSend[200];

    // Evita desbordamiento con snprintf
    sprintf(commandToSend, "sendRF/%s/%s/", type, commandStr);

    esp_now_send_data(lcd, (uint8_t *) commandToSend, sizeof(commandToSend));

    while (!hasSent()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    if (getResult() == ESP_OK) {
        showConfirmationPopup(senderRFInstance, "Command was sent");
    } else {
        showConfirmationPopup(senderRFInstance, "Error sending command");
        ESP_LOGE(TAG, "Command was not sent");
    }
}
