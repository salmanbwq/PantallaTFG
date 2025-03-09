//
// Created by salman on 25/01/25.
//

#include "lvgl.h"
#include "GarageRfScreen.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <portmacro.h>
#include <string.h>
#include <ESPNOW/espNow.h>
#include <sys/stat.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/RF/Receive/ReceiveRfScreen.h>
#include <ui/RF/Send/SendRfScreen.h>
#include <ui/RF/Utils/Commands/Receiver/RFCommandReceiver.h>
#include <ui/RF/Utils/Commands/Sender/RFCommandSender.h>
#include <ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.h>
#include <ui/RF/Utils/JSONManager/RFDataStore.h>

static const char *TAG = "GarageRfScreen";
static lv_obj_t *garageRfScrn;
static char instanceName[100];

typedef enum Command {
    OPEN, CLOSE
} Command;

void goToGarageScreen(const char *name, const DispositiveSelectorType type) {
    initializeFlags();
    deletePreviousScreen(garageRfScrn);
    ESP_LOGI(TAG, "Going to Garage Screen");
    garageRfScreen(name, type);
    lv_scr_load(garageRfScrn);
}


static void sendCommandRF(lv_event_t *event) {
    const Command command = *(Command *) lv_event_get_user_data(event);
    switch (command) {
        case OPEN:
            ESP_LOGI(TAG, "Sending Open Command");
            sendRFCommand("Open");
            break;
        case CLOSE:
            ESP_LOGI(TAG, "Sending Close Command");
            sendRFCommand("Close");
            break;
    }
}

static void receiveCommandRF(lv_event_t *event) {
    const Command command = *(Command *) lv_event_get_user_data(event);
    switch (command) {
        case OPEN:
            ESP_LOGI(TAG, "Receiving Open Command");
            receiveRFCommand("Open");
            break;
        case CLOSE:
            ESP_LOGI(TAG, "Receiving Close Command");
            receiveRFCommand("Close");
            break;
    }
}

static void garageRfScreen(const char *name, const DispositiveSelectorType type) {
    strcpy(instanceName, name);
    static Command open = OPEN;
    static Command close = CLOSE;
    // Crear la pantalla
    garageRfScrn = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(garageRfScrn, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(garageRfScrn, LV_OPA_COVER, LV_PART_MAIN);

    // Etiqueta de título en la parte superior
    lv_obj_t *titleLabel = lv_label_create(garageRfScrn);
    lv_label_set_text_fmt(titleLabel, "%s", name); // Mostrar nombre e ID
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0x000000), 0);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 20); // Margen superior pequeño

    // Contenedor para los botones
    lv_obj_t *btnContainer = lv_obj_create(garageRfScrn);
    lv_obj_set_size(btnContainer, 140, 120); // Ajustado para la pantalla horizontal
    lv_obj_set_style_bg_opa(btnContainer, LV_OPA_0, 0); // Hacer transparente
    lv_obj_align(btnContainer, LV_ALIGN_CENTER, 0, -10);

    // Estilo de los botones
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_bg_color(&style_btn, lv_color_hex(0x007BFF)); // Azul moderno
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_border_color(&style_btn, lv_color_hex(0xFFFFFF));
    lv_style_set_shadow_width(&style_btn, 5);
    lv_style_set_pad_all(&style_btn, 10); // Relleno para mejor apariencia

    // Botón "Abrir" (arriba)
    lv_obj_t *btnOpen = lv_btn_create(btnContainer);
    lv_obj_add_style(btnOpen, &style_btn, 0);
    lv_obj_set_size(btnOpen, 100, 40); // Botón ancho para facilidad de uso
    lv_obj_align(btnOpen, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_t *btnOpen_label = lv_label_create(btnOpen);
    lv_label_set_text(btnOpen_label, LV_SYMBOL_UP " ABRIR");
    lv_obj_center(btnOpen_label);

    // Botón "Cerrar" (abajo)
    lv_obj_t *btnClose = lv_btn_create(btnContainer);
    lv_obj_add_style(btnClose, &style_btn, 0);
    lv_obj_set_size(btnClose, 100, 40);
    lv_obj_align(btnClose, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_t *btnClose_label = lv_label_create(btnClose);
    lv_label_set_text(btnClose_label, LV_SYMBOL_DOWN " CERRAR");
    lv_obj_center(btnClose_label);

    // Botón de "Volver" (abajo)
    lv_obj_t *rtrnbtn = lv_btn_create(garageRfScrn);
    lv_obj_set_size(rtrnbtn, 150, 40);
    lv_obj_align(rtrnbtn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_color(rtrnbtn, lv_color_hex(0xFF0000), 0);
    lv_obj_set_style_radius(rtrnbtn, 10, 0);

    lv_obj_t *rtrnbtnLabel = lv_label_create(rtrnbtn);
    lv_label_set_text(rtrnbtnLabel, LV_SYMBOL_LEFT " Volver");
    lv_obj_center(rtrnbtnLabel);


    switch (type) {
        case SENDER:
            ESP_LOGI(TAG, "SENDER");
        // Change callbacks to sender
            initializeRFSender(instanceName, garageRfScrn);
            lv_obj_add_event_cb(btnOpen, sendCommandRF, LV_EVENT_CLICKED, &open);
            lv_obj_add_event_cb(btnClose, sendCommandRF, LV_EVENT_CLICKED, &close);
            lv_obj_add_event_cb(rtrnbtn, goToSendRfScreen, LV_EVENT_CLICKED, NULL);
            break;
        case RECEIVER:
            // Changer callbacks to receiver
            ESP_LOGI(TAG, "RECEIVER");
            initializeRFReceiver(instanceName, garageRfScrn);
            lv_obj_add_event_cb(btnOpen, receiveCommandRF, LV_EVENT_CLICKED, &open);
            lv_obj_add_event_cb(btnClose, receiveCommandRF, LV_EVENT_CLICKED, &close);
            lv_obj_add_event_cb(rtrnbtn, goToReceiveRFscreen, LV_EVENT_CLICKED, NULL);
            break;
        default:
            ESP_LOGI(TAG, "UNKNOWN TYPE");
    }
}
