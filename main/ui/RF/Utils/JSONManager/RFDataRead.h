//
// Created by salman on 11/02/25.
//

#ifndef RFDATAREAD_H
#define RFDATAREAD_H
#include <cJSON/cJSON.h>

static char *getGarageCommand(const char *name, const char *commanName, cJSON *json, cJSON *commands);

static char *getAlarmCommand(const char *name, const char *commanName, cJSON *json, cJSON *commands);

static cJSON *getDeviceFromJson(const char *deviceName);

/**
 * Method to get Device Type
 * @param name Device name
 * @return Device type
 */
char *getDeviceType(char *name);

/**
 * Method to get Device command
 * @param name Device name
 * @param commanName Command name
 * @return Command
 */
char *getCommandsFromJSON(const char *name, const char *commanName);


#endif //RFDATAREAD_H
