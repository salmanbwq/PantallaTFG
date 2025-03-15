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

#include "CommonUI/InterfacesUtils.h"
#include "IR/IRScreen.h"
#include "RF/RfScreen.h"
#include "Settings/SettingsScreen.h"

static lv_obj_t *mainInstance;

void goToMainScreen() {
    deletePreviousScreen(mainInstance);
    ESP_LOGI("mainScreen", "Going to main screen");
    mainScreenInit();
    lv_scr_load(mainInstance);
}


void mainScreenInit(void) {
    mainInstance = lv_obj_create(NULL);
    lv_scr_load(mainInstance);
    lv_obj_set_style_bg_color(mainInstance, lv_color_hex(0xc5d9db), 0);


    // ✅ Establecer fondo blanco
    lv_obj_set_style_bg_opa(mainInstance, LV_OPA_COVER, LV_PART_MAIN);

    // 📌 Crear un contenedor para los botones con diseño de cuadrícula
    lv_obj_t *gridContainer = lv_obj_create(mainInstance);
    lv_obj_set_size(gridContainer, 240, 180); // Aumentamos la altura para más espacio
    lv_obj_center(gridContainer);
    lv_obj_set_style_bg_color(gridContainer, lv_color_hex(0xffffff), 0);
    // 📌 Configuración de la cuadrícula (2 columnas, 2 filas, más espacio entre botones)
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(gridContainer, col_dsc, row_dsc);
    lv_obj_set_style_pad_row(gridContainer, 20, 0); // Aumentar espacio entre filas
    lv_obj_set_style_pad_column(gridContainer, 20, 0); // Aumentar espacio entre columnas

    // 📌 Crear un estilo para los botones
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_bg_color(&style_btn, lv_color_hex(0x007BFF)); // Azul
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_border_color(&style_btn, lv_color_hex(0xFFFFFF));
    lv_style_set_shadow_width(&style_btn, 10);

    // 📌 Crear los botones con iconos
    lv_obj_t *btnRF = lv_btn_create(gridContainer);
    lv_obj_add_style(btnRF, &style_btn, 0);
    lv_obj_set_grid_cell(btnRF, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *btnRF_icon = lv_label_create(btnRF);
    lv_label_set_text(btnRF_icon, LV_SYMBOL_WIFI "\nRF");
    lv_obj_center(btnRF_icon);
    lv_obj_add_event_cb(btnRF, goToRfScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnIR = lv_btn_create(gridContainer);
    lv_obj_add_style(btnIR, &style_btn, 0);
    lv_obj_set_grid_cell(btnIR, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *btnIR_icon = lv_label_create(btnIR);
    lv_label_set_text(btnIR_icon, LV_SYMBOL_BLUETOOTH "\nIR");
    lv_obj_center(btnIR_icon);
    lv_obj_add_event_cb(btnIR, goToIRScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnRFID = lv_btn_create(gridContainer);
    lv_obj_add_style(btnRFID, &style_btn, 0);
    lv_obj_set_grid_cell(btnRFID, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *btnRFID_icon = lv_label_create(btnRFID);
    lv_label_set_text(btnRFID_icon, LV_SYMBOL_SD_CARD "\nRFID");
    lv_obj_center(btnRFID_icon);
    lv_obj_add_event_cb(btnRFID, goToRfidScreen, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btnSettings = lv_btn_create(gridContainer);
    lv_obj_add_style(btnSettings, &style_btn, 0);
    lv_obj_set_grid_cell(btnSettings, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *btnSettings_icon = lv_label_create(btnSettings);
    lv_label_set_text(btnSettings_icon, LV_SYMBOL_SETTINGS "\nConfig");
    lv_obj_center(btnSettings_icon);
    lv_obj_add_event_cb(btnSettings, goToSettings, LV_EVENT_CLICKED, NULL);
}
