//
// Created by salman on 8/02/25.
//

#ifndef RFDATASTORE_H
#define RFDATASTORE_H

#define MAX_DEVICES 10
#define MAX_COMMANDS 4
#include <esp_err.h>
#include <misc/lv_types.h>

void init_spiffs();

esp_err_t updateRFJSON(char *name, const char *commandName, char *content);


void saveRfDispositives(lv_event_t *indev);
#endif //RFDATASTORE_H
