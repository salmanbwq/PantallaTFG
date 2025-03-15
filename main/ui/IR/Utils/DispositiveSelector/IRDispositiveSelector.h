//
// Created by salman on 21/02/25.
//

#ifndef IRDISPOSITIVESELECTOR_H
#define IRDISPOSITIVESELECTOR_H
#include <misc/lv_types.h>
#include <ui/IR/Utils/Enums.h>

/**
 * Method to get device type in enum type
 * @param deviceName device Name
 * @return Device type
 */
IRDeviceType getIRType(const char *deviceName);

/**
 * Method to handle devices selected.
 * @param deviceType device type
 * @param dispositiveName device name
 * @param commandType command type: SENDER or RECEIVER
 */
void changeToIRDeviceTypeScreen(const char *deviceType, const char *dispositiveName,
                                const IRActionType commandType);

#endif //IRDISPOSITIVESELECTOR_H
