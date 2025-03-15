//
// Created by bnsal on 30/09/2024.
//

#ifndef ESPNOW_H
#define ESPNOW_H
#include "esp_now.h"
#endif //ESPNOW_H

static uint8_t lcd[ESP_NOW_ETH_ALEN] = {0x40, 0x22, 0xd8, 0x1e, 0x6c, 0x9c};

/**
 * Method to initilize wifi module
 * @return ESP_OK
 */
esp_err_t init_wifi(void);

/**
 * Callback receiver
 * @param esp_now_info
 * @param data
 * @param data_len
 */
void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len);

/**
 *Callback sender
 * @param mac_addr
 * @param status
 */
void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);


/**
 * Initializer esp_now method
 * @return
 */
esp_err_t init_esp_now(esp_now_recv_cb_t recv);

/**
 *  Peer register method
 * @param peer_addr
 * @return
 */
esp_err_t register_peer(uint8_t *peer_addr);

/**
 * Method to send data (uint8_t)
 * @param peer_addr
 * @param data
 * @param len
 * @return
 */
esp_err_t esp_now_send_data(const uint8_t *peer_addr, const uint8_t *data, size_t len);

/**
 * Method to reset all flags to default values
 */
void initializeFlags();

/**
 * Method to get sender flag
 * @return Sender flag
 */
bool hasSent();

/**
 * Method to get receiver flag
 * @return Receiver flag
 */
bool hasReceived();

/**
 * Method to get Buffer
 * @return Buffer
 */
char *getBuffer();

/**
 * Method to get sender result
 * @return Sender result {ESP_OK, ESP_FAIL}
 */
esp_err_t getResult();

/**
 * Method to handle esp-now receiver data.
 * @param esp_now_info
 * @param data
 * @param data_len
 */
void receive(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len);
