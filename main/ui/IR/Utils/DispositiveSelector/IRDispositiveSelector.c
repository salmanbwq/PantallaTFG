//
// Created by salman on 21/02/25.
//e

#include "IRDispositiveSelector.h"

#include <esp_log.h>
#include <string.h>
#include <ui/IR/DevicesTypes/AC/AC.h>
#include <ui/IR/DevicesTypes/Lamp/LampIR.h>
#include <ui/IR/DevicesTypes/TV/TVIRScreen.h>
#include <ui/IR/Utils/Enums.h>
static const char *TAG = "IRDispositiveSelector";

IRDeviceType getIRType(const char *command) {
    if (strcmp(command, "TV") == 0) return TV;
    if (strcmp(command, "A/C") == 0) return A_C;
    if (strcmp(command, "Lamp") == 0) return LAMP;
    return UNKNOWN;
}


void changeToIRDeviceTypeScreen(const char *deviceType, const char *dispositiveName,
                                const IRActionType commandType) {
    switch (getIRType(deviceType)) {
        case TV:
            ESP_LOGI(TAG, "Se ha seleccionado %s de tipo %s", dispositiveName, deviceType);
            goToTvIRScreen(dispositiveName, commandType);
            break;
        case A_C:
            ESP_LOGI(TAG, "Se ha seleccionado %s de tipo %s", dispositiveName, deviceType);
            goToACScreen(dispositiveName, commandType);
            break;
        case LAMP:
            ESP_LOGI(TAG, "Se ha seleccionado %s de tipo %s", dispositiveName, deviceType);
            goToLampScreen(dispositiveName, commandType);
        default:
            ESP_LOGI(TAG, "Se ha seleccionado una instancia no valida %s", deviceType);
    }
}
