//
// Created by salman on 20/02/25.
//

#include "RFDropDown.h"

#include <esp_log.h>
#include <string.h>
#include <ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.h>
#include <ui/RF/Utils/JSONManager/RFDataRead.h>

#include "lvgl.h"

static const char *TAG = "RFDropDown";
static char selectedText[20];
static lv_obj_t *dropDwnHndlr;

void dropdownEventHandlerRF(lv_event_t *event) {
    const DispositiveSelectorType type = *(DispositiveSelectorType *) lv_event_get_user_data(event);
    switch (type) {
        case SENDER:
            ESP_LOGI(TAG, "SENDER");
            break;
        default: ;
    }
    dropDwnHndlr = lv_event_get_target(event);
    if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED) {
        lv_dropdown_get_selected_str(dropDwnHndlr, selectedText, sizeof(selectedText));
        ESP_LOGI(TAG, "%s", selectedText);
        // Obtener el tipo de dispositivo desde el JSON
        char *deviceType = malloc(20 * sizeof(char));;
        strcpy(deviceType, getDeviceType(selectedText));

        if (deviceType) {
            ESP_LOGI(TAG, "Se ha seleccionado: %s, Tipo: %s", selectedText, deviceType);
            changeToDeviceTypeScreen(event, deviceType, selectedText, type);
        } else {
            ESP_LOGE(TAG, "No se encontró el tipo de dispositivo en JSON.");
        }
    }
}
