//
// Created by salman on 7/03/25.
//

#ifndef RFCOMMANDRECEIVER_H
#define RFCOMMANDRECEIVER_H
#include <misc/lv_types.h>

void initializeRFReceiver(char instance[100], lv_obj_t *instanceObj);

void receiveRFCommand(const char *commandName);
#endif //RFCOMMANDRECEIVER_H
