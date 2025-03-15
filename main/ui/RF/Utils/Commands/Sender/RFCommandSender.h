//
// Created by salman on 7/03/25.
//

#ifndef RFCOMMANDSENDER_H
#define RFCOMMANDSENDER_H
#include <misc/lv_types.h>

/**
 * Method to initialize RF Sender
 * @param instance Instance name
 * @param instanceObj Instance object
 */
void initializeRFSender(char instance[100], lv_obj_t *instanceObj);

/**
 * Method to send RF command
 * @param commandName Command to be sent
 */
void sendRFCommand(const char *commandName);

#endif //RFCOMMANDSENDER_H
