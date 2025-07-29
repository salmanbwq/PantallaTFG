//
// Created by salman on 15/02/25.
//

#include "ConfirmationPopup.h"
#include "lvgl.h"


static void onOkClicked(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *popup = lv_obj_get_parent(btn);


    if (popup) {
        lv_obj_del(popup);
    }
}


void showConfirmationPopup(lv_obj_t *parent, const char *message) {

    lv_obj_t *popup = lv_obj_create(parent);
    lv_obj_set_size(popup, 200, 100);
    lv_obj_align(popup, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(popup, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_radius(popup, 10, 0);


    lv_obj_t *label = lv_label_create(popup);
    lv_label_set_text(label, message);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);


    lv_obj_t *btn_ok = lv_btn_create(popup);
    lv_obj_set_size(btn_ok, 80, 30);
    lv_obj_align(btn_ok, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_color(btn_ok, lv_color_hex(0x00bb2d), 0);

    lv_obj_t *label_ok = lv_label_create(btn_ok);
    lv_label_set_text(label_ok, "Aceptar");
    lv_obj_center(label_ok);


    lv_obj_add_event_cb(btn_ok, onOkClicked, LV_EVENT_CLICKED, NULL);
}
