//
// Created by salman on 24/02/25.
//

#ifndef RESET_H
#define RESET_H
#include <misc/lv_types.h>

typedef enum JsonTypes {
    RF, RFID, IR, ALL, UNKNOWN
} JsonTypes;

static void resetScreen();

void goToResetScreen(lv_event_t *event);
#endif //RESET_H
