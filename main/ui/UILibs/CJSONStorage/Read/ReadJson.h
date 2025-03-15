//
// Created by salman on 15/02/25.
//

#ifndef READJSON_H
#define READJSON_H
#include <cJSON/cJSON.h>
#include <misc/lv_types.h>

/**
 * This method populate dropdown passed by parameter
 * @param dropdw Dropdown to populate
 * @param FILE_PATH Json file path were devices are stored
 */
void populateDropdownNames(lv_obj_t *dropdw, const char *FILE_PATH);

/**
 * Method that returns devices stored in Json.
 * @param FILE_PATH Json file path where devices are stored
 * @return Devices
 */
cJSON *readDevices(const char *FILE_PATH);

/**
 * 
 * @param name Dispositive name
 * @param FILE_PATH Json file path where device is stored
 * @return The device Json content
 */
cJSON *getJsonByName(const char *name, const char *FILE_PATH);

#endif //READJSON_H
