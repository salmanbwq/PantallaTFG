//
// Created by salman on 2/03/25.
//

#ifndef LAMPIR_H
#define LAMPIR_H
#include <ui/IR/Utils/Enums.h>

/**
 * Method to call Lamp screen
 * @param instance_name Instance name
 * @param type Instance type
 */
void goToLampScreen(const char *instance_name, const IRActionType type);

static void lampScreen();
#endif //LAMPIR_H
