//
// Created by salman on 25/01/25.
//

#include <esp_log.h>
#include <string.h>

#include "IRScreen.h"
#include "lvgl.h"
#include "SendIrScreen.h"

#include <ui/GeneralObjects/InterfacesUtils.h>

#include "SendTvIrScreen.h"


void goToIrSend(lv_indev_t *indev) {
    deletePreviousScreen(sendIrScrn);
    ESP_LOGI("IRScreen", "Going to send IR");
    sendIrScreen();
    lv_scr_load(sendIrScrn);
}


static void dropdownEventHandler(lv_event_t *event) {
    dropdwnHndlr = lv_event_get_target(event);
    if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED) {
        selectedId = lv_dropdown_get_selected(dropdwnHndlr);

        lv_dropdown_get_selected_str(dropdwnHndlr, selectedText, sizeof(selectedText));

        if (strcmp(selectedText, "Tv") == 0) {
            sendTvIrScreen();
            lv_scr_load(sendTvIrScrn);
            ESP_LOGI(ID, "Tv screen loaded");
        } else if (strcmp(selectedText, "Bombilla") == 0) {
            //lv_screen_load(lightRfScrn);
            ESP_LOGI(ID, "Bombilla screen loaded");
        } else if (strcmp(selectedText, "Alarma") == 0) {
            // lv_screen_load(alarmRfScrn);
            ESP_LOGI(ID, "Alarm screen loaded");
        }
    }
}

static void sendIrScreen() {
    sendIrScrn = lv_obj_create(NULL);

    lv_obj_t *rtrnBtn = lv_btn_create(sendIrScrn);
    lv_obj_t *rtrnLabel = lv_label_create(rtrnBtn);

    lv_obj_t *ddDisp = lv_dropdown_create(sendIrScrn);
    lv_dropdown_set_options(ddDisp, "Tv\nA/C\nXbox");
    lv_obj_add_event_cb(ddDisp, dropdownEventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_set_pos(ddDisp, 100, 30);
    lv_obj_set_size(ddDisp, 100, 30);

    lv_obj_set_style_bg_color(rtrnBtn, lv_color_hex(0xff0000), 0);
    lv_label_set_text(rtrnLabel, "Return");
    lv_obj_center(rtrnLabel);

    lv_obj_set_pos(rtrnBtn, 75, 100);
    lv_obj_set_size(rtrnBtn, 150, 30);

    lv_obj_add_event_cb(rtrnBtn, goToIRScreen, LV_EVENT_CLICKED, NULL);
}
