//
// Created by salman on 7/02/25.
//

#ifndef IRSCREEN_H
#define IRSCREEN_H
#include "lvgl.h"

/**
 * Method to change to IR Screen
 * @param indev Arguments passed by callback method
 */
void goToIRScreen(lv_indev_t *indev);

static void irScreen();

#endif //IRSCREEN_H
