//
// Created by salman on 23/01/25.
//
#include <esp_log.h>

#include "lvgl.h"
#include <stdio.h>
#include <ui/CommonUI/InterfacesUtils.h>

#include "AddIrDispScreen.h"

#include <ui/IR/Utils/JSONManager/IRDataStore.h>
#include <ui/Settings/SettingsScreen.h>

static lv_obj_t *addIrScreen;
static lv_obj_t *keyboard;
static lv_obj_t *ta_name;
static lv_obj_t *dd_type;

void goToAddIrDispScreen(lv_event_t *event) {
    deletePreviousScreen(addIrScreen);
    ESP_LOGI("AddIrDispScreen", "Going to add ir disp screen");
    addIrDispScreen();
    lv_scr_load(addIrScreen);
}

static void textAreaHandlerIR(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);

    if (code == LV_EVENT_FOCUSED) {
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Mostrar teclado
        lv_obj_add_flag(dd_type, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea(keyboard, ta);
        lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 20, 40);

        // Mover el cuadro de texto hacia arriba para que no lo tape el teclado
    } else if (code == LV_EVENT_DEFOCUSED) {
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Ocultar teclado
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_clear_flag(dd_type, LV_OBJ_FLAG_HIDDEN);

        // Restaurar la posición original del cuadro de texto
        lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 20, 110);
    }
}


// Función para crear la pantalla de la interfaz gráfica
static void addIrDispScreen() {
    addIrScreen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(addIrScreen, lv_color_hex(0xc5d9db), 0);

    lv_obj_set_size(addIrScreen, 320, 240); // Tamaño de la pantalla
    lv_obj_set_style_bg_opa(addIrScreen, LV_OPA_COVER, 0);

    // Dropdown para el tipo de dispositivo
    dd_type = lv_dropdown_create(addIrScreen);
    lv_dropdown_set_options(dd_type, "TV\nA/C\nLamp");
    lv_obj_set_width(dd_type, 140); // Ajustar ancho
    lv_obj_align(dd_type, LV_ALIGN_TOP_LEFT, 20, 40); // Posición en la esquina superior izquierda

    // Textarea para ingresar el nombre
    ta_name = lv_textarea_create(addIrScreen);
    lv_textarea_set_placeholder_text(ta_name, "Nombre del dispositivo");
    lv_textarea_set_max_length(ta_name, 10);
    lv_obj_set_width(ta_name, 140); // Ajustar ancho
    lv_obj_set_height(ta_name, 50);
    lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 20, 110); // Posición relativa a los dropdowns
    lv_obj_add_event_cb(ta_name, textAreaHandlerIR, LV_EVENT_ALL,NULL);


    keyboard = lv_keyboard_create(addIrScreen);
    lv_obj_set_size(keyboard, 320, 100);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);


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
