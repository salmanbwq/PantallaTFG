//
// Created by salman on 7/02/25.
//

#ifndef SENDTVIRSCREEN_H
#define SENDTVIRSCREEN_H
#include <ui/IR/Utils/Enums.h>



/**
 * This  method is in charge of switching to TV IR Screen
 * @param instance_name DispositiveName
 * @param type Command type to apply, SENDER or RECEIVER
 */
void goToTvIRScreen(const char *instance_name, const IRActionType type);

static void tvIRScreen();
#endif //SENDTVIRSCREEN_H
