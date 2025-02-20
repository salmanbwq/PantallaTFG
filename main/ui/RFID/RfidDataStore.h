//
// Created by salman on 15/02/25.
//

#ifndef RFIDDATASTORE_H
#define RFIDDATASTORE_H
#include <esp_err.h>
#include <misc/lv_types.h>

void saveRFIDDispositives(lv_event_t *indev);

esp_err_t updateRFIDJSON(char *name, char *commandName, char *content);

void populateDropdownCommands(lv_event_t *indev);


#endif //RFIDDATASTORE_H
