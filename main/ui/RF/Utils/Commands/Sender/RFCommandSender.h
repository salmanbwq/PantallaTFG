//
// Created by salman on 7/03/25.
//

#ifndef RFCOMMANDSENDER_H
#define RFCOMMANDSENDER_H
#include <misc/lv_types.h>

void initializeRFSender(char instance[100], lv_obj_t *instanceObj);

void sendRFCommand(const char *commandName);

#endif //RFCOMMANDSENDER_H
