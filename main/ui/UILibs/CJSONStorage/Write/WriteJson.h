//
// Created by salman on 15/02/25.
//

#ifndef WRITEJSON_H
#define WRITEJSON_H
#include <cJSON/cJSON.h>

/**
 * Method to add new device
 * @param new_device Device Json to be added.
 * @param FILE_PATH Json path where device will be stored.
 */
void AddDevicesJson(cJSON *new_device, const char *FILE_PATH);

/**
 * Method to update Devices Json
 * @param json Device Json to be updated
 * @param FILE_PATH Json path for each device
 */
void updateDevicesListJson(const cJSON *json, const char *FILE_PATH);
#endif //WRITEJSON_H
