//
// Created by salman on 2/03/25.
//

#ifndef IRCOMMANDSENDER_H
#define IRCOMMANDSENDER_H
#include <misc/lv_types.h>

/**
 * Method to initialize sender
 * @param instance Instance name
 * @param instanceObj Instance object
 */
void initializeSender(char instance[100], lv_obj_t *instanceObj);

/**
 * Method to send command
 * @param e Arguments passed to sender
 */
void sendCommand(lv_event_t *e);

#endif //IRCOMMANDSENDER_H
