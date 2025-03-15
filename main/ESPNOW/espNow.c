//
// Created by bnsal on 30/09/2024.
//


#include "espNow.h"
#include <esp_log.h>
#include <esp_mac.h>
#include <esp_netif.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <string.h>

#define ESP_CHANNEL 11
#define BUFFER_SIZE 256 // Definir tamaño adecuado

static char bufferLectura[BUFFER_SIZE]; // Definir el buffer correctamente


//  ADDR ESP32
static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x40, 0x22, 0xd8, 0x1e, 0x6c, 0x9c};

//  ADDR LCD
//static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x24, 0xdc, 0xc3, 0x49, 0x5e, 0x10};
static bool received = false;
static bool sent = false;
static esp_err_t result = ESP_OK;

esp_err_t init_wifi(void) {
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

    esp_netif_init();
    esp_event_loop_create_default();
    nvs_flash_init();
    esp_wifi_init(&wifi_init_config);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    // 🔥 CAMBIAR CANAL A 11
    ESP_ERROR_CHECK(esp_wifi_set_channel(ESP_CHANNEL, WIFI_SECOND_CHAN_NONE));

    ESP_LOGI("WIFI_INIT", "📡 WiFi inicializado en canal 11");
    return ESP_OK;
}

void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len) {
    ESP_LOGI("ESP_RECV", "Data recibida" MACSTR "%s", MAC2STR(esp_now_info->src_addr), data);
}

void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        ESP_LOGI("ESP_SEND", "ESP_NOW_SEND_SUCCESS");
        sent = true;
        result = ESP_OK;
    } else {
        ESP_LOGI("ESP_SEND", "ESP_NOW_SEND_FAIL");
        sent = true;
        received = true;
        result = ESP_FAIL;
    }
}

esp_err_t init_esp_now(esp_now_recv_cb_t recv) {
    esp_now_init();
    esp_now_register_recv_cb(recv);
    esp_now_register_send_cb(send_cb);

    ESP_LOGI("ESP_NOW_INIT", "Esp_NOW inicializado");
    return ESP_OK;
}

esp_err_t register_peer(uint8_t *peer_addr) {
    esp_now_peer_info_t esp_now_peer_info = {};
    memcpy(esp_now_peer_info.peer_addr, peer_mac, ESP_NOW_ETH_ALEN);
    esp_now_peer_info.channel = ESP_CHANNEL;
    esp_now_peer_info.ifidx = ESP_IF_WIFI_STA;
    esp_now_add_peer(&esp_now_peer_info);
    return ESP_OK;
}

esp_err_t esp_now_send_data(const uint8_t *peer_addr, const uint8_t *data, size_t len) {
    esp_now_send(peer_addr, data, len);
    return ESP_OK;
}

void receive(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len) {
    printf("Recibiendo \n");
    ESP_LOGI("ESP_RECV", "Data recibida desde " MACSTR ": %.*s", MAC2STR(esp_now_info->src_addr), data_len,
             (const char *)data);
    int copy_len = data_len < BUFFER_SIZE - 1 ? data_len : BUFFER_SIZE - 1;
    strncpy(bufferLectura, (const char *) data, copy_len);
    bufferLectura[copy_len] = '\0'; // Asegurar terminación de string
    ESP_LOGI("ESP_RECV", "Lectura: %s", bufferLectura);
    received = true;
}


bool hasReceived() {
    return received;
}

bool hasSent() {
    return sent;
}

void initializeFlags() {
    bufferLectura[0] = '\0';
    received = false;
    sent = false;
}

char *getBuffer() {
    return bufferLectura;
}

esp_err_t getResult() {
    return result;
}
