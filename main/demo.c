#include <stdio.h>
#include <math.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_system.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_lvgl_port.h>
#include <esp_now.h>
#include <string.h>
#include <display/lv_display.h>
#include <ui/MainScreen.h>
#include <ui/UILibs/CJSONStorage/JSONManager.h>

#include "ESPNOW/espNow.h"

#include "lcd.h"
#include "touch.h"

static const char *TAG = "demo";


#define FILE_PATH "/spiffs/irDevices.json"

void print_json_from_spiffs() {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        ESP_LOGE("SPIFFS", "No se pudo abrir el archivo JSON");
        return;
    }

    // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    if (fileSize <= 0) {
        ESP_LOGE("SPIFFS", "El archivo JSON está vacío o corrupto.");
        fclose(file);
        return;
    }

    // Asignar memoria dinámica para leer el archivo completo
    char *buffer = (char *) malloc(fileSize + 1);
    if (!buffer) {
        ESP_LOGE("SPIFFS", "No se pudo asignar memoria para leer el JSON.");
        fclose(file);
        return;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0'; // Asegurar terminación de string

    fclose(file);

    ESP_LOGI("SPIFFS", "Contenido del JSON:\n%s", buffer);

    free(buffer); // Liberar memoria después de usarla
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

    init_spiffs();
    print_json_from_spiffs();
    mainScreenInit();

    while (42) {
        vTaskDelay(125 / portTICK_PERIOD_MS);
    }
}
