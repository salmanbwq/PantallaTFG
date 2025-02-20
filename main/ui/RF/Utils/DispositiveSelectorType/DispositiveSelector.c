//
// Created by salman on 20/02/25.
//

#include "DispositiveSelector.h"

#include <esp_log.h>
#include <string.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/RF/DevicesTypes/Alarm/AlarmRfScreen.h>
#include <ui/RF/DevicesTypes/Garage/GarageRfScreen.h>
#include <ui/RF/DevicesTypes/Light/LightRfScreen.h>

#include "lvgl.h"

static const char *TAG = "DispositiveSelector";

static InstanceType get_command_type(const char *command) {
    if (strcmp(command, "Garage") == 0) return Garage;
    if (strcmp(command, "alarm") == 0) return ALARM;
    if (strcmp(command, "light") == 0) return LIGHT;
    return UNKNOWN_COMMAND;
}


static void loadAlarmScreen(void) {
    if (!alarmRfScrn) {
        alarmRfScrn = lv_obj_create(NULL);
    }
    deletePreviousScreen(alarmRfScrn);
    alarmRfScreen();
    lv_scr_load(alarmRfScrn);
}

static void loadLightScreen(void) {
    if (!lightRfScrn) {
        lightRfScrn = lv_obj_create(NULL);
    }
    deletePreviousScreen(lightRfScrn);
    ligthRfScreen();
    lv_scr_load(lightRfScrn);
}

void changeToDeviceTypeScreen(lv_event_t *event, const char *deviceType, const char *selectedText,
                              const DispositiveSelectorType type) {
    switch (type) {
        case SENDER:
            ESP_LOGI(TAG, "SENDER %s", selectedText);
            break;
        default:
            ESP_LOGI(TAG, "Uknown");
            break;
    }


    switch (get_command_type(deviceType)) {
        case Garage:
            ESP_LOGI(TAG, "Se ha seleccionado: %s", deviceType);
            goToGarageScreen(event, selectedText, type);
            break;
        case ALARM:
            ESP_LOGI(TAG, "Se ha seleccionado: %s", deviceType);
            loadLightScreen();
            break;
        case LIGHT:
            ESP_LOGI(TAG, "Se ha seleccionado: %s", deviceType);
            loadAlarmScreen();
            break;
        default:
            ESP_LOGI(TAG, "Se ha seleccionado una instancia no valida %s", deviceType);
    }
}
