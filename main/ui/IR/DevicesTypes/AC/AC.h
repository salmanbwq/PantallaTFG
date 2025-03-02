//
// Created by salman on 24/02/25.
//

#ifndef AC_H
#define AC_H
#include <ui/IR/Utils/Enums.h>

/**
 * Method to change to A/C screen
 * @param instance_name Dispositive name
 * @param type Command type, SENDER or RECEIVER
 */
void goToACScreen(const char *instance_name, const IRActionType type);

static void ACScreen();
#endif //AC_H
