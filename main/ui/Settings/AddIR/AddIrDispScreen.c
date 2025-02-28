//
// Created by salman on 23/01/25.
//
#include <esp_log.h>

#include "lvgl.h"
#include <stdio.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/CommonUI/Keyboard.h>

#include "AddIrDispScreen.h"

#include <ui/IR/Utils/JSONManager/IRDataStore.h>
#include <ui/Settings/SettingsScreen.h>

static lv_obj_t *addIrScreen;

void goToAddIrDispScreen(lv_event_t *event) {
    deletePreviousScreen(addIrScreen);
    ESP_LOGI("AddIrDispScreen", "Going to add ir disp screen");
    addIrDispScreen();
    lv_scr_load(addIrScreen);
}


// Función para crear la pantalla de la interfaz gráfica
static void addIrDispScreen() {
    addIrScreen = lv_obj_create(NULL);
    lv_obj_set_size(addIrScreen, 320, 240); // Tamaño de la pantalla
    lv_obj_set_style_bg_color(addIrScreen, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(addIrScreen, LV_OPA_COVER, 0);

    // Dropdown para el tipo de dispositivo
    lv_obj_t *dd_type = lv_dropdown_create(addIrScreen);
    lv_dropdown_set_options(dd_type, "TV\nA/C");
    lv_obj_set_width(dd_type, 140); // Ajustar ancho
    lv_obj_align(dd_type, LV_ALIGN_TOP_LEFT, 20, 40); // Posición en la esquina superior izquierda

    createKeyboard(addIrScreen);

    // Textarea para ingresar el nombre
    lv_obj_t *ta_name = lv_textarea_create(addIrScreen);
    lv_textarea_set_placeholder_text(ta_name, "Nombre del dispositivo");
    lv_textarea_set_max_length(ta_name, 10);
    lv_obj_set_width(ta_name, 140); // Ajustar ancho
    lv_obj_set_height(ta_name, 50);
    lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 20, 110); // Posición relativa a los dropdowns
    lv_obj_add_event_cb(ta_name, textAreaHandler, LV_EVENT_ALL,NULL);


    // Botón para guardar el dispositivo
    lv_obj_t *btn_save = lv_btn_create(addIrScreen);
    lv_obj_set_size(btn_save, 80, 30); // Ajustar tamaño
    lv_obj_set_style_bg_color(btn_save, lv_color_hex(0x00bb2d), 0);
    lv_obj_align(btn_save, LV_ALIGN_BOTTOM_LEFT, 210, -150); // Posición en la esquina inferior izquierda

    lv_obj_t **widgets = malloc(2 * sizeof(lv_obj_t *));
    widgets[0] = dd_type;
    widgets[1] = ta_name;

    lv_obj_t *btn_label_save = lv_label_create(btn_save);
    lv_label_set_text(btn_label_save, "Guardar");
    lv_obj_center(btn_label_save);
    lv_obj_add_event_cb(btn_save, saveIRDispositives, LV_EVENT_CLICKED, widgets);

    // Botón para volver a la pantalla RF
    lv_obj_t *btn_back = lv_btn_create(addIrScreen);
    lv_obj_set_size(btn_back, 80, 30); // Ajustar tamaño
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0xff0000), 0);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 210, -100); // Posición en la esquina inferior derecha

    lv_obj_t *btn_label_back = lv_label_create(btn_back);
    lv_label_set_text(btn_label_back, "Volver");
    lv_obj_center(btn_label_back);
    lv_obj_add_event_cb(btn_back, goToSettings, LV_EVENT_CLICKED, NULL);
}
