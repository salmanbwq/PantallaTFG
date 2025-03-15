//
// Created by salman on 7/02/25.
//

#ifndef RECEIVERFIDSCREEN_H
#define RECEIVERFIDSCREEN_H
#include "lvgl.h"

/**
 * Method to call Receive RFID Screen
 */
void goToReceiveRfidScreen();

static void receiveRfidScreen(void);

static void receiveRFIDData(lv_event_t *event);


#endif //RECEIVERFIDSCREEN_H
