//
// Created by salman on 8/02/25.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <misc/lv_types.h>

/**
 * Method to handle text area events
 * @param e keyboard
 */
void textAreaHandler(lv_event_t *e);

/**
 * Method to create keyboard on a given screen
 * @param parent screen where keyboard will be created
 */
void createKeyboard(lv_obj_t *parent);
#endif //KEYBOARD_H
