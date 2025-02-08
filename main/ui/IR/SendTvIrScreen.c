//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "SendIrScreen.h"
#include "SendTvIrScreen.h"


void sendTvIrScreen() {
    sendTvIrScrn = lv_obj_create(NULL);

    lv_obj_t *grid = lv_obj_create(sendTvIrScrn);
    lv_obj_t *rtrnBtn = lv_btn_create(sendTvIrScrn);
    lv_obj_t *rtrnLbl = lv_label_create(rtrnBtn);

    lv_obj_set_size(grid, 140, 200);

    lv_obj_set_pos(grid, 75, 10);

    // Definir las áreas del grid
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    // 3 columnas iguales
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    // 3 filas iguales
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

    // Crear botones
    lv_obj_t *btn_on = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_on, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *label_on = lv_label_create(btn_on);
    lv_label_set_text(label_on, "ON");

    lv_obj_t *btn_off = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_off, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_t *label_off = lv_label_create(btn_off);
    lv_label_set_text(label_off, "OFF");

    lv_obj_t *btn_plus = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_plus, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *label_plus = lv_label_create(btn_plus);
    lv_label_set_text(label_plus, "+");

    lv_obj_t *btn_minus = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_minus, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_t *label_minus = lv_label_create(btn_minus);
    lv_label_set_text(label_minus, "-");

    lv_obj_t *btn_up = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_up, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *label_up = lv_label_create(btn_up);
    lv_label_set_text(label_up, "↑");

    lv_obj_t *btn_down = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_down, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_t *label_down = lv_label_create(btn_down);
    lv_label_set_text(label_down, "↓");

    lv_obj_t *btn_left = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_left, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *label_left = lv_label_create(btn_left);
    lv_label_set_text(label_left, "←");

    lv_obj_t *btn_right = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_right, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *label_right = lv_label_create(btn_right);
    lv_label_set_text(label_right, "→");

    lv_obj_t *btn_ok = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn_ok, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *label_ok = lv_label_create(btn_ok);
    lv_label_set_text(label_ok, "OK");

    lv_obj_set_pos(rtrnBtn, 75, 240);
    lv_obj_set_size(rtrnBtn, 150, 30);
    lv_label_set_text(rtrnLbl, "Return");
    lv_obj_center(rtrnLbl);
    lv_obj_add_event_cb(rtrnBtn, goToIrSend, LV_EVENT_CLICKED,NULL);
}
