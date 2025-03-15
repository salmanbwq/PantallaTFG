//
// Created by salman on 2/03/25.
//

#ifndef IRCOMMANDRECEIVER_H
#define IRCOMMANDRECEIVER_H
#include <misc/lv_types.h>

/**
 * Method to initialize receiver.
 * @param instance Instance name
 * @param instanceObj Instance Object
 */
void initializeReceiver(char instance[100], lv_obj_t *instanceObj);

/**
 * Method to receive command
 * @param e parameters given to receiver: command name...
 */
void receiveCommand(lv_event_t *e);

#endif //IRCOMMANDRECEIVER_H
