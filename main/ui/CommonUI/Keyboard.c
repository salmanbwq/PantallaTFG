//
// Created by salman on 8/02/25.
//

#include "Keyboard.h"
#include "lvgl.h"

static lv_obj_t *keyboard;

void textAreaHandler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);

    if (code == LV_EVENT_FOCUSED) {
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea(keyboard, ta);

    } else if (code == LV_EVENT_DEFOCUSED) {
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea(keyboard, NULL);

    }
}


void createKeyboard(lv_obj_t *parent) {
    keyboard = lv_keyboard_create(parent);
    lv_obj_set_size(keyboard, 320, 100);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}
