//
// Created by salman on 7/02/25.
//
//
// Created by salman on 27/11/24.
//
#include <esp_log.h>
#include <esp_lvgl_port.h>
#include "lvgl.h"
#include <ui/RFID/RfidScreen.h>
#include <ui/MainScreen.h>

#include "GeneralObjects/InterfacesUtils.h"
#include "IR/IRScreen.h"
#include "RF/RfScreen.h"
#include "Settings/SettingsScreen.h"

void goToMainScreen(lv_event_t *event) {
    deletePreviousScreen(mainScrn);
    ESP_LOGI("mainScreen", "Going to main screen");
    mainScreenInit();
    lv_scr_load(mainScrn);
}

void mainScreenInit(void) {
    mainScrn = lv_obj_create(NULL); // Crear la pantalla principal
    lv_scr_load(mainScrn); // Cargar la pantalla

    // Crear un contenedor para los botones con diseño de cuadrícula
    lv_obj_t *gridContainer = lv_obj_create(mainScrn);
    lv_obj_set_size(gridContainer, 240, 120); // Ajusta al tamaño de la pantalla
    lv_obj_center(gridContainer);

    // Configuración de la cuadrícula (2 columnas, 2 filas)
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(gridContainer, col_dsc, row_dsc);

    // Crear los botones y añadirlos al contenedor
    lv_obj_t *btnRF = lv_btn_create(gridContainer);
    lv_obj_set_grid_cell(btnRF, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *btnRFlb = lv_label_create(btnRF);
    lv_label_set_text(btnRFlb, "RF");
    lv_obj_add_event_cb(btnRF, goToRfScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnIR = lv_btn_create(gridContainer);
    lv_obj_set_grid_cell(btnIR, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *btnIRlb = lv_label_create(btnIR);
    lv_label_set_text(btnIRlb, "IR");
    lv_obj_add_event_cb(btnIR, goToIRScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnRFID = lv_btn_create(gridContainer);
    lv_obj_set_grid_cell(btnRFID, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *btnRFIDlb = lv_label_create(btnRFID);
    lv_label_set_text(btnRFIDlb, "RFID");
    lv_obj_add_event_cb(btnRFID, goToRfidScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnSettings = lv_btn_create(gridContainer);
    lv_obj_set_grid_cell(btnSettings, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *btnSettingslb = lv_label_create(btnSettings);
    lv_label_set_text(btnSettingslb, "Settings");
    lv_obj_add_event_cb(btnSettings, goToSettings, LV_EVENT_CLICKED, NULL);
}
