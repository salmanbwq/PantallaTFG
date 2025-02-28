//
// Created by salman on 21/02/25.
//

#ifndef IRDATASTORE_H
#define IRDATASTORE_H
#include <esp_err.h>
#include <misc/lv_types.h>

void saveIRDispositives(lv_event_t *indev);

esp_err_t updateIRJSON(char *name, const char *commandName, char *content);

#endif //IRDATASTORE_H
