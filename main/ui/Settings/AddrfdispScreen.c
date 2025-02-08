//
// Created by salman on 24/11/24.
//
#include <esp_log.h>

#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include <ui/MainScreen.h>
#include <ui/GeneralObjects/InterfacesUtils.h>
#include <ui/GeneralObjects/Keyboard.h>

#include "AddRfDispScreen.h"
#include "SettingsScreen.h"
#include <ui/RF/RFDataStore.h>

void goToAddRfDispScreen(lv_event_t *event) {
    deletePreviousScreen(addrfScreen);
    ESP_LOGI("Addrfdisp", "Going to AddRfDispScreen");
    addrfdispScreen();
    lv_scr_load(addrfScreen);
}


// Función para crear la pantalla de la interfaz gráfica
static void addrfdispScreen() {
    addrfScreen = lv_obj_create(NULL);
    lv_obj_set_size(addrfScreen, 320, 240); // Tamaño de la pantalla
    lv_obj_set_style_bg_color(addrfScreen, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(addrfScreen, LV_OPA_COVER, 0);

    // Dropdown para el tipo de dispositivo
    lv_obj_t *dd_type = lv_dropdown_create(addrfScreen);
    lv_dropdown_set_options(dd_type, "Sensor\nGarage\nLamp");
    lv_obj_set_width(dd_type, 140); // Ajustar ancho
    lv_obj_align(dd_type, LV_ALIGN_TOP_LEFT, 20, 20); // Posición en la esquina superior izquierda

    // Dropdown para la frecuencia
    lv_obj_t *dd_freq = lv_dropdown_create(addrfScreen);
    lv_dropdown_set_options(dd_freq, "433 MHz\n868 MHz\n2.4 GHz");
    lv_obj_set_width(dd_freq, 140); // Ajustar ancho
    lv_obj_align(dd_freq, LV_ALIGN_TOP_LEFT, 20, 80); // Posición relativa al primer dropdown


    // Textarea para ingresar el nombre
    lv_obj_t *ta_name = lv_textarea_create(addrfScreen);
    lv_textarea_set_placeholder_text(ta_name, "Nombre del dispositivo");
    lv_textarea_set_max_length(ta_name, 31);
    lv_obj_set_width(ta_name, 140); // Ajustar ancho
    lv_obj_set_height(ta_name, 50);
    lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 20, 140); // Posición relativa a los dropdowns
    lv_obj_add_event_cb(ta_name, textAreaHandler, LV_EVENT_ALL, NULL);

    createKeyboard(addrfScreen);


    // Botón para guardar el dispositivo
    lv_obj_t *btn_save = lv_btn_create(addrfScreen);
    lv_obj_set_size(btn_save, 80, 30); // Ajustar tamaño
    lv_obj_set_style_bg_color(btn_save, lv_color_hex(0x00bb2d), 0);
    lv_obj_align(btn_save, LV_ALIGN_BOTTOM_LEFT, 210, -150); // Posición en la esquina inferior izquierda

    lv_obj_t *btn_label_save = lv_label_create(btn_save);
    lv_label_set_text(btn_label_save, "Guardar");
    lv_obj_center(btn_label_save);

    // Botón para volver a la pantalla RF
    lv_obj_t *btn_back = lv_btn_create(addrfScreen);
    lv_obj_set_size(btn_back, 80, 30); // Ajustar tamaño
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0xff0000), 0);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 210, -100); // Posición en la esquina inferior derecha

    lv_obj_t *btn_label_back = lv_label_create(btn_back);
    lv_label_set_text(btn_label_back, "Volver");
    lv_obj_center(btn_label_back);
    lv_obj_add_event_cb(btn_back, goToSettings, LV_EVENT_CLICKED, NULL);

    lv_obj_t **widgets = malloc(2 * sizeof(lv_obj_t *));
    widgets[0] = dd_type;
    widgets[1] = ta_name;

    lv_obj_add_event_cb(btn_save, saveRfDispositives, LV_EVENT_CLICKED, widgets);
}
