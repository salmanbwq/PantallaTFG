//
// Created by salman on 15/02/25.
//

#ifndef WRITEJSON_H
#define WRITEJSON_H
#include <cJSON/cJSON.h>

void AddDevicesJson(cJSON *new_device, const char *FILE_PATH);

cJSON *getJsonByName(const char *name, const char *FILE_PATH);

void updateDevicesListJson(const cJSON *json, const char *FILE_PATH);
#endif //WRITEJSON_H
