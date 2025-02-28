//
// Created by salman on 27/02/25.
//

#include "IRDropdown.h"

#include <esp_log.h>
#include <string.h>
#include <ui/IR/Utils/DispositiveSelector/IRDispositiveSelector.h>
#include <ui/IR/Utils/JSONManager/IRDataRead.h>

#include "lvgl.h"

static char selectedText[20];

static uint16_t selectedId;

void dropdownEventHandler(lv_event_t *event) {
    const lv_obj_t *dropdown = lv_event_get_target(event);

    if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED) {
        IRActionType action = *(IRActionType *) lv_event_get_user_data(event);
        selectedId = lv_dropdown_get_selected(dropdown);
        char *type = malloc(50 * sizeof(char));
        lv_dropdown_get_selected_str(dropdown, selectedText, sizeof(selectedText));
        strcpy(type, getIRDeviceType(selectedText));

        changeToIRDeviceTypeScreen(type, selectedText, action);
    }
}
