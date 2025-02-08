//
// Created by salman on 24/11/24.
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON/cJSON.h"
#include "lvgl.h"
#include "SendRfScreen.h"

#include <esp_log.h>
#include <stdio.h>
#include <ui/GeneralObjects/InterfacesUtils.h>
#include <ui/Settings/AddRfidDispScreen.h>
#include <ui/Settings/SettingsScreen.h>

#include "AlarmRfScreen.h"
#include "GarageRfScreen.h"
#include "LightRfScreen.h"
#include "RFDataStore.h"
#include "RfScreen.h"

void goToSendRfScreen(lv_event_t *event) {
    deletePreviousScreen(settingsScrn);
    ESP_LOGI("RfScreen", "Going to Sending RfScreen");
    sendRfScreen();
    lv_scr_load(sendRfScrn);
}

static void dropdownEventHandlerIR(lv_event_t *event) {
    dropdwnHndlr = lv_event_get_target(event);
    if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED) {
        selectedId = lv_dropdown_get_selected(dropdwnHndlr);

        lv_dropdown_get_selected_str(dropdwnHndlr, selectedText, sizeof(selectedText));

        if (strcmp(selectedText, "Garage") == 0) {
            ESP_LOGI(ID, "Se ha seleccionado Grage");
            deletePreviousScreen(garageRfScrn);
            garageRfScreen();
            lv_scr_load(garageRfScrn);
        } else if (strcmp(selectedText, "Bombilla") == 0) {
            ESP_LOGI(ID, "Se ha seleccionado Bombilla");
            deletePreviousScreen(lightRfScrn);
            ligthRfScreen();
            lv_scr_load(lightRfScrn);
        } else if (strcmp(selectedText, "Alarma") == 0) {
            deletePreviousScreen(alarmRfScrn);
            alarmRfScreen();
            ESP_LOGI(ID, "Se ha seleccionado Alarma");
            lv_scr_load(alarmRfScrn);
        }
    }
}

static void populateDropdown(lv_obj_t *dropdw) {
    lv_dropdown_clear_options(dropdw);
    char name[70];
    for (int i = 0; i < rfDispCount; i++) {
        snprintf(name, sizeof(name), "%s (%s)", devicesRf[i].name, devicesRf[i].id);
        lv_dropdown_add_option(dropdw, name, i);
    }
}

static void sendRfScreen(void) {
    sendRfScrn = lv_obj_create(NULL);


    lv_obj_t *rtrngrg = lv_btn_create(sendRfScrn);


    lv_obj_set_size(sendRfScrn, 150, 50);
    lv_obj_set_pos(sendRfScrn, 75, 100);


    // Dropdown para el tipo de dispositivo
    lv_obj_t *dd_type = lv_dropdown_create(sendRfScrn);
    populateDropdown(dd_type);
    lv_dropdown_set_text(dd_type, "Select Disp");
    lv_obj_set_width(dd_type, 150); // Ajustar ancho
    lv_obj_set_pos(dd_type, 75, 30); // Posición en la esquina superior izquierda
    lv_obj_add_event_cb(dd_type, dropdownEventHandlerIR, LV_EVENT_ALL,NULL);

    lv_obj_set_size(rtrngrg, 150, 50);
    lv_obj_set_pos(rtrngrg, 75, 100);
    lv_obj_set_style_bg_color(rtrngrg, lv_color_hex(0xff0000), 0);
    lv_obj_t *rtrnLbl = lv_label_create(rtrngrg);
    lv_label_set_text(rtrnLbl, "Return");
    lv_obj_center(rtrnLbl);
    lv_obj_add_event_cb(rtrngrg, goToRfScreen, LV_EVENT_CLICKED, NULL);
}
