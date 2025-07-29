#include <esp_log.h>
#include "Loading.h"
#include "lvgl.h"

static lv_obj_t *loading_popup = NULL;
static lv_timer_t *loading_timer = NULL;
static bool loading_done = false;

static void closeLoadingPopup(void) {
    if (loading_timer) {
        lv_timer_del(loading_timer);
        loading_timer = NULL;
    }

    if (loading_popup) {
        lv_obj_del(loading_popup);
        loading_popup = NULL;
    }
}

static void checkLoadingStatus(lv_timer_t *timer) {
    if (loading_done) {
        ESP_LOGI("UI", "Carga completada, cerrando popup");
        closeLoadingPopup();
    }
}

void showLoadingPopup(lv_obj_t *parent) {
    if (!parent) {
        ESP_LOGE("UI", "Parent es NULL, no se puede mostrar el popup");
        return;
    }

    loading_done = false;


    loading_popup = lv_obj_create(parent);
    lv_obj_set_size(loading_popup, 200, 100);
    lv_obj_align(loading_popup, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(loading_popup, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_radius(loading_popup, 50, LV_PART_MAIN);


    lv_obj_t *spinner = lv_spinner_create(loading_popup);
    lv_obj_set_size(spinner, 50, 50);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0);

    loading_timer = lv_timer_create(checkLoadingStatus, 500, NULL);
}

void setLoadingDone(bool done) {
    ESP_LOGI("UI", "setLoadingDone: %d", done);
    loading_done = done;
}
