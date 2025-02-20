//
// Created by salman on 8/02/25.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <misc/lv_types.h>
static lv_obj_t *keyboard;

void textAreaHandler(lv_event_t *e);

#endif //KEYBOARD_H
void createKeyboard(lv_obj_t *parent);
