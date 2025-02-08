//
// Created by salman on 7/02/25.
//

#ifndef MAINSCREEN_H
#define MAINSCREEN_H
#include <esp_now.h>
#include "lvgl.h"


void mainScreenInit(void);

void goToMainScreen(lv_event_t *event);


static lv_obj_t *mainScrn;
#endif //MAINSCREEN_H
