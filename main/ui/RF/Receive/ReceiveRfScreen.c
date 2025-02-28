//
// Created by salman on 23/01/25.
//

#include "lvgl.h"
#include "ReceiveRfScreen.h"

#include <esp_log.h>
#include <ui/CommonUI/InterfacesUtils.h>
#include <ui/RF/RfScreen.h>
#include <ui/RF/Utils/Enums.h>
#include <ui/RF/Utils/Dropdown/RFDropDown.h>
#include <ui/UILibs/CJSONStorage/Read/ReadJson.h>
#include <ui/UILibs/Popup/Confirmation/ConfirmationPopup.h>


#define FILE_PATH "/spiffs/rf_devices.json"


void goToReceiveRFscreen(lv_event_t *event) {
    deletePreviousScreen(receiveRfScrn);
    ESP_LOGI("ReceiveRfScreen", "Going to receive RF screen");
    receiveRfScreen();

    lv_scr_load(receiveRfScrn);
}


static void receiveRfScreen(void) {
    static DispositiveSelectorType dispositive_selector = RECEIVER;
    receiveRfScrn = lv_obj_create(NULL);

    lv_obj_t *rtrngrg = lv_btn_create(receiveRfScrn);


    lv_obj_set_size(receiveRfScrn, 150, 50);
    lv_obj_set_pos(receiveRfScrn, 75, 100);


    // Dropdown para el tipo de dispositivo
    lv_obj_t *dd_type = lv_dropdown_create(receiveRfScrn);
    populateDropdownNames(dd_type, FILE_PATH);
    lv_obj_set_width(dd_type, 150); // Ajustar ancho
    lv_obj_set_pos(dd_type, 75, 30); // Posición en la esquina superior izquierda
    lv_obj_add_event_cb(dd_type, dropdownEventHandlerRF, LV_EVENT_VALUE_CHANGED, &dispositive_selector);


    if (lv_dropdown_get_option_count(dd_type) == 0) {
        showConfirmationPopup(receiveRfScrn, "Data empty");
        lv_obj_add_state(dd_type, LV_STATE_DISABLED);
    }

    lv_obj_set_size(rtrngrg, 150, 50);
    lv_obj_set_pos(rtrngrg, 75, 100);
    lv_obj_set_style_bg_color(rtrngrg, lv_color_hex(0xff0000), 0);
    lv_obj_t *rtrnLbl = lv_label_create(rtrngrg);
    lv_label_set_text(rtrnLbl, "Return");
    lv_obj_center(rtrnLbl);
    lv_obj_add_event_cb(rtrngrg, goToRfScreen, LV_EVENT_CLICKED, NULL);
}
