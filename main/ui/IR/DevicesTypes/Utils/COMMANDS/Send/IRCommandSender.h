//
// Created by salman on 2/03/25.
//

#ifndef IRCOMMANDSENDER_H
#define IRCOMMANDSENDER_H
#include <misc/lv_types.h>

void initializeSender(char instance[100], lv_obj_t *instanceObj);

void sendCommand(lv_event_t *e);

#endif //IRCOMMANDSENDER_H
