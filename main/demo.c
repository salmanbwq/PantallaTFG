#include <stdio.h>
#include <math.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_system.h>
#include <esp_log.h>
#include <esp_err.h>
#include <lvgl.h>
#include <esp_lvgl_port.h>
#include <esp_mac.h>
#include <esp_now.h>
#include <string.h>
#include <ui/MainScreen.h>

#include "ESPNOW/espNow.h"

#include "lcd.h"
#include "touch.h"

static const char *TAG = "demo";


static void receive(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len) {
    printf("Recibiendo \n");
    ESP_LOGI("ESP_RECV", "Data recibida desde " MACSTR ": %.*s", MAC2STR(esp_now_info->src_addr), data_len,
             (const char *)data);
    char received_data[50];
    strncpy(received_data, (const char *) data, sizeof(received_data) - 1);
    received_data[sizeof(received_data) - 1] = '\0';
    const char *command = strtok(received_data, "/");
}

void app_main(void) {
    esp_lcd_panel_io_handle_t lcd_io;
    esp_lcd_panel_handle_t lcd_panel;
    esp_lcd_touch_handle_t tp;
    lvgl_port_touch_cfg_t touch_cfg;
    lv_display_t *lvgl_display = NULL;

    ESP_ERROR_CHECK(lcd_display_brightness_init());

    ESP_ERROR_CHECK(app_lcd_init(&lcd_io, &lcd_panel));
    lvgl_display = app_lvgl_init(lcd_io, lcd_panel);
    if (lvgl_display == NULL) {
        ESP_LOGI(TAG, "fatal error in app_lvgl_init");
        esp_restart();
    }

    ESP_ERROR_CHECK(touch_initilization(&tp));
    touch_cfg.disp = lvgl_display;
    touch_cfg.handle = tp;
    lvgl_port_add_touch(&touch_cfg);

    ESP_ERROR_CHECK(lcd_display_brightness_set(75));
    ESP_ERROR_CHECK(lcd_display_rotate(lvgl_display, LV_DISPLAY_ROTATION_90));


    ESP_ERROR_CHECK(init_wifi());
    ESP_ERROR_CHECK(init_esp_now(receive));
    ESP_ERROR_CHECK(register_peer(lcd));


    mainScreenInit();

    while (42) {
        vTaskDelay(125 / portTICK_PERIOD_MS);
    }
}
