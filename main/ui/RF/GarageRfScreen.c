//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "GarageRfScreen.h"

#include "RfScreen.h"
#include "SendRfScreen.h"


void garageRfScreen(void) {
    garageRfScrn = lv_obj_create(NULL);

    lv_obj_t *rtrnbtn = lv_btn_create(garageRfScrn);
    lv_obj_t *rtrnbtnLabel = lv_label_create(rtrnbtn);
    lv_obj_t *gridContainer = lv_obj_create(garageRfScrn);

    lv_obj_set_size(gridContainer, 150, 120); // Ajusta al tamaño de la pantalla
    lv_obj_set_pos(gridContainer, 70, 30);

    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(gridContainer, col_dsc, row_dsc);

    // Crear los botones y añadirlos al contenedor
    lv_obj_t *btnRF = lv_btn_create(gridContainer);
    lv_obj_set_grid_cell(btnRF, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *btnRFlb = lv_label_create(btnRF);
    lv_label_set_text(btnRFlb, "1");
    //lv_obj_add_event_cb(btnRF, changetoRFScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnIR = lv_btn_create(gridContainer);
    lv_obj_set_grid_cell(btnIR, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *btnIRlb = lv_label_create(btnIR);
    lv_label_set_text(btnIRlb, "2");
    //lv_obj_add_event_cb(btnIR, sendRFBackend, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnRFID = lv_btn_create(gridContainer);
    lv_obj_set_grid_cell(btnRFID, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *btnRFIDlb = lv_label_create(btnRFID);
    lv_label_set_text(btnRFIDlb, "3");
    //lv_obj_add_event_cb(btnRFID, changetoRFIDScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnSettings = lv_btn_create(gridContainer);
    lv_obj_set_grid_cell(btnSettings, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *btnSettingslb = lv_label_create(btnSettings);
    lv_label_set_text(btnSettingslb, "4");
    //lv_obj_add_event_cb(btnSettings, goToSettingsScreen, LV_EVENT_CLICKED, NULL);


    lv_obj_set_size(rtrnbtn, 150, 50);
    lv_obj_set_pos(rtrnbtn, 70, 170);
    lv_label_set_text(rtrnbtnLabel, "Return");
    lv_obj_set_style_bg_color(rtrnbtn, lv_color_hex(0xff0000), 0);
    lv_obj_center(rtrnbtnLabel);
    lv_obj_add_event_cb(rtrnbtn, goToSendRfScreen, LV_EVENT_CLICKED, NULL);
}
