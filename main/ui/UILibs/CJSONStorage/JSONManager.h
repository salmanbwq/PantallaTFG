//
// Created by salman on 15/02/25.
//

#ifndef JSONMANAGER_H
#define JSONMANAGER_H
#include <cJSON/cJSON.h>

/**
 * Method to add new devices
 * @param new_device Device Json to be stored
 * @param FILE_PATH Json path where device will be stored
 */
void AddDevices(cJSON *new_device, const char *FILE_PATH);

/**
 * Initialize method for JSON.
 */
void init_spiffs();
#endif //JSONMANAGER_H
