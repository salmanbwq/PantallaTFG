//
// Created by salman on 23/01/25.
//

#include "lvgl.h"
#include "ReceiveRfScreen.h"

#include <esp_log.h>
#include <ui/GeneralObjects/InterfacesUtils.h>

#include "RfScreen.h"

void goToReceiveRFscreen(lv_event_t *event) {
    deletePreviousScreen(receiveRfScrn);
    ESP_LOGI("ReceiveRfScreen", "Going to receive RF screen");
    receiveRfScreen();

    lv_scr_load(receiveRfScrn);
}


static void receiveRfScreen(void) {
    receiveRfScrn = lv_img_create(NULL);
    lv_obj_set_size(receiveRfScrn, 320, 240); // Tamaño de la pantalla
    lv_obj_set_style_bg_color(receiveRfScrn, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(receiveRfScrn, LV_OPA_COVER, 0);

    // Dropdown para el tipo de dispositivo
    lv_obj_t *dd_type = lv_dropdown_create(receiveRfScrn);
    lv_dropdown_set_options(dd_type, "Sensor\nGarage\nLamp");
    lv_obj_set_width(dd_type, 140); // Ajustar ancho
    lv_obj_align(dd_type, LV_ALIGN_TOP_LEFT, 20, 50); //

    // Textarea para ingresar el nombre
    lv_obj_t *ta_name = lv_textarea_create(receiveRfScrn);
    lv_textarea_set_placeholder_text(ta_name, "Nombre del comando");
    lv_textarea_set_max_length(ta_name, 10);
    lv_obj_set_width(ta_name, 140); // Ajustar ancho
    lv_obj_set_height(ta_name, 50);
    lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 20, 105); // Posición relativa a los dropdowns

    // Botón para guardar el dispositivo
    lv_obj_t *btn_save = lv_btn_create(receiveRfScrn);
    lv_obj_set_size(btn_save, 80, 30); // Ajustar tamaño
    lv_obj_set_style_bg_color(btn_save, lv_color_hex(0x00bb2d), 0);
    lv_obj_align(btn_save, LV_ALIGN_BOTTOM_LEFT, 210, -150); // Posición en la esquina inferior izquierda


    lv_obj_t *btn_label_save = lv_label_create(btn_save);
    lv_label_set_text(btn_label_save, "Recibir");
    lv_obj_center(btn_label_save);

    // Botón para volver a la pantalla RF
    lv_obj_t *btn_back = lv_btn_create(receiveRfScrn);
    lv_obj_set_size(btn_back, 80, 30); // Ajustar tamaño
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0xff0000), 0);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 210, -100); // Posición en la esquina inferior derecha

    lv_obj_t *btn_label_back = lv_label_create(btn_back);
    lv_label_set_text(btn_label_back, "Volver");
    lv_obj_center(btn_label_back);
    lv_obj_add_event_cb(btn_back, goToRfScreen, LV_EVENT_CLICKED, NULL);
}
