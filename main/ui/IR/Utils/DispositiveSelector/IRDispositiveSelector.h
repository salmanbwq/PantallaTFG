//
// Created by salman on 21/02/25.
//

#ifndef IRDISPOSITIVESELECTOR_H
#define IRDISPOSITIVESELECTOR_H
#include <misc/lv_types.h>
#include <ui/IR/Utils/Enums.h>

IRDeviceType getIRType(const char *command);

void changeToIRDeviceTypeScreen(const char *deviceType, const char *dispositiveName,
                                const IRActionType commandType);

#endif //IRDISPOSITIVESELECTOR_H
