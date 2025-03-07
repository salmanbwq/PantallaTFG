//
// Created by salman on 2/03/25.
//

#include "IRCommandReceiver.h"
#include <freertos/FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <portmacro.h>

#include <esp_log.h>
#include <string.h>
#include <ESPNOW/espNow.h>
#include <freertos/task.h>
#include <ui/IR/Utils/JSONManager/IRDataStore.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>

#include "lvgl.h"

static char instanceName[100];
static lv_obj_t *receiverRFInstance;

static char *TAG = "Receive IR command";

void initializeReceiver(char instance[100], lv_obj_t *instanceObj) {
    if (instanceObj == NULL || strlen(instance) == 0) {
        ESP_LOGE(TAG, "Sender initialization failed");
        return;
    }

    strcpy(instanceName, instance);
    receiverRFInstance = instanceObj;
    ESP_LOGI(TAG, "Receiver initialized with %s info", instanceName);
}

void receiveCommand(lv_event_t *e) {
    initializeFlags();

    const char *txt = (const char *) lv_event_get_user_data(e);
    if (!txt || txt[0] == '\0') {
        ESP_LOGE(TAG, "El comando recibido está vacío");
        return;
    }
    ESP_LOGI(TAG, "Botón %s presionado\n", txt);

    char cmd[100] = "receiveIR/";
    esp_now_send_data(lcd, (uint8_t *) cmd, strlen(cmd) + 1);

    while (!hasReceived()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    ESP_LOGI(TAG, "Comando IR recibido");

    if (getResult() == ESP_OK) {
        char buffer[10] = {0}; // Inicializa en 0 para evitar datos residuales
        const char *receivedBuffer = getBuffer();

        if (!receivedBuffer || receivedBuffer[0] == '\0') {
            ESP_LOGE(TAG, "No se recibió ningún comando");
            showConfirmationPopup(receiverRFInstance, "Comando vacío");
            return;
        }

        strncpy(buffer, receivedBuffer, sizeof(buffer) - 1); // Copia segura

        // Verificar si la clave JSON realmente existe antes de actualizar
        if (updateIRJSON(instanceName, txt, buffer) == ESP_OK) {
            showConfirmationPopup(receiverRFInstance, "Comando guardado");
        } else {
            showConfirmationPopup(receiverRFInstance, "Error al guardar comando");
            ESP_LOGE(TAG, "Error al actualizar el JSON");
        }
    } else {
        showConfirmationPopup(receiverRFInstance, "Error al recibir comando");
        ESP_LOGE(TAG, "El comando no fue recibido correctamente");
    }
}
