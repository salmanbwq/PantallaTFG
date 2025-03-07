//
// Created by salman on 2/03/25.
//

#include "IRCommandSender.h"
#include <freertos/FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <portmacro.h>

#include <esp_log.h>
#include <string.h>
#include <ESPNOW/espNow.h>
#include <freertos/task.h>
#include <ui/IR/Utils/JSONManager/IRDataRead.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

#include "lvgl.h"

static char instanceName[100];
static lv_obj_t *sendIRInstance;

static char *TAG = "JSON Sender IR";

void initializeSender(char instance[100], lv_obj_t *instanceObj) {
    if (instanceObj == NULL || strlen(instance) == 0) {
        ESP_LOGE(TAG, "Sender initialization failed");
        return;
    }

    strcpy(instanceName, instance);
    sendIRInstance = instanceObj;
    ESP_LOGI(TAG, "Sender initialized with %s info", instanceName);
}


void sendCommand(lv_event_t *e) {
    initializeFlags();
    const lv_obj_t *btn = lv_event_get_target(e);
    const char *txt = lv_label_get_text(lv_obj_get_child(btn, 0));

    if (txt == NULL) {
        ESP_LOGE(TAG, "Error: Button text is NULL");
        return;
    }

    ESP_LOGI(TAG, "Button %s pressed\n", txt);

    // Validar que user_data no sea NULL
    const char *user_data = lv_event_get_user_data(e);
    if (user_data == NULL) {
        ESP_LOGE(TAG, "Error: No command name received");
        return;
    }

    char commandName[100];
    strncpy(commandName, user_data, sizeof(commandName) - 1);
    commandName[sizeof(commandName) - 1] = '\0'; // Asegurar terminación de cadena

    char buffer[10];
    char *aux = getIRCommand(instanceName, commandName);
    if (aux == NULL) {
        ESP_LOGE(TAG, "Error: Command %s not found", commandName);
        showConfirmationPopup(sendIRInstance, "No command stored");
        free(aux);
        return;
    }
    strcpy(buffer, aux);
    free(aux);
    ESP_LOGI(TAG, "Sending IR command: %s", buffer);

    char cmd[100];
    snprintf(cmd, sizeof(cmd), "sendIr/%s/", buffer); // Protege contra desbordamiento

    esp_now_send_data(lcd, (uint8_t *) cmd, strlen(cmd) + 1); // Incluye '\0' final

    while (!hasSent()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    if (getResult() == ESP_OK) {
        showConfirmationPopup(sendIRInstance, "Command was sent");
    } else {
        showConfirmationPopup(sendIRInstance, "Error sending command");
        ESP_LOGE(TAG, "Command was not sent");
    }
}
