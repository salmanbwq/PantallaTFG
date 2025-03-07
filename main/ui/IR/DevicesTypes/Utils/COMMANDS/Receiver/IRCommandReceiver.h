//
// Created by salman on 2/03/25.
//

#ifndef IRCOMMANDRECEIVER_H
#define IRCOMMANDRECEIVER_H
#include <misc/lv_types.h>

void initializeReceiver(char instance[100], lv_obj_t *instanceObj);

void receiveCommand(lv_event_t *e);

#endif //IRCOMMANDRECEIVER_H
