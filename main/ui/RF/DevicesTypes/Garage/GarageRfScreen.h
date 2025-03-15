//
// Created by salman on 7/02/25.
//

#ifndef GARAGERFSCREEN_H
#define GARAGERFSCREEN_H
#include <ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.h>

/**
 * Method to load Garage Screen
 * @param name Device name
 * @param type Type of instnace {SENDER, RECEIVER}
 */
void goToGarageScreen(const char *name, const DispositiveSelectorType type);

static void sendCommandRF(lv_event_t *event);

static void receiveCommandRF(lv_event_t *event);

static void garageRfScreen(const char *name, const enum DispositiveSelectorType type);

#endif //GARAGERFSCREEN_H
