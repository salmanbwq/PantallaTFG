//
// Created by salman on 11/02/25.
//

#ifndef RFDATAREAD_H
#define RFDATAREAD_H
#include <stdbool.h>
#include <cJSON/cJSON.h>

typedef struct {
    char rf[32];
    char command1[32];
    char command2[32];
} CommandData;


char *getDeviceType(char *name);

int getDeviceID(char *name);

CommandData *getCommandsFromJSON(const char *id);


#endif //RFDATAREAD_H
