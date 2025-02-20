//
// Created by salman on 7/02/25.
//

#ifndef GARAGERFSCREEN_H
#define GARAGERFSCREEN_H
#include <ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.h>



void goToGarageScreen(lv_event_t *event, const char *name, const DispositiveSelectorType type);

static void garageRfScreen(const char *name, const enum DispositiveSelectorType type);

#endif //GARAGERFSCREEN_H
