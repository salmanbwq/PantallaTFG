//
// Created by salman on 7/03/25.
//

#ifndef RFCOMMANDRECEIVER_H
#define RFCOMMANDRECEIVER_H
#include <misc/lv_types.h>

/**
 * Method to initialize RF receiver
 * @param instance Instance name
 * @param instanceObj Instance object
 */
void initializeRFReceiver(char instance[100], lv_obj_t *instanceObj);

/**
 * Method to receive RF command
 * @param commandName Command to be received
 */
void receiveRFCommand(const char *commandName);
#endif //RFCOMMANDRECEIVER_H
