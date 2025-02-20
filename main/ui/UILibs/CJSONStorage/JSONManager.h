//
// Created by salman on 15/02/25.
//

#ifndef JSONMANAGER_H
#define JSONMANAGER_H
#include <cJSON/cJSON.h>

void AddDevices(cJSON *new_device, const char *FILE_PATH);

void init_spiffs();
#endif //JSONMANAGER_H
