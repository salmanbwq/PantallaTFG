//
// Created by salman on 15/02/25.
//

#ifndef READJSON_H
#define READJSON_H
#include <cJSON/cJSON.h>
#include <misc/lv_types.h>

void populateDropdownNames(lv_obj_t *dropdw, const char *FILE_PATH);

cJSON *readDevices(const char *FILE_PATH);

cJSON *getJsonByName(const char *name, const char *FILE_PATH);

#endif //READJSON_H
