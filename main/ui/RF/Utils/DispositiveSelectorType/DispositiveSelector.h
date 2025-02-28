//
// Created by salman on 20/02/25.
//

#ifndef DISPOSITIVESELECTOR_H
#define DISPOSITIVESELECTOR_H
#include <misc/lv_types.h>
#include "ui/RF/Utils/Enums.h"

typedef enum InstanceType {
    Garage,
    LIGHT,
    ALARM,
    UNKNOWN_COMMAND
} InstanceType;

InstanceType get_command_type(const char *command);


void changeToRFDeviceTypeScreen(const char *deviceType, const char *selectedText,
                                const DispositiveSelectorType type);
#endif //DISPOSITIVESELECTOR_H
