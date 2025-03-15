//
// Created by salman on 15/02/25.
//

#ifndef RFIDDATASTORE_H
#define RFIDDATASTORE_H
#include <esp_err.h>
#include <misc/lv_types.h>

/**
 * Method to add new RFID Devices.
 * @param indev Arguments to add new RFID device
 */
void saveRFIDDispositives(lv_event_t *indev);

/**
 * Method to update device Json with new information
 * @param name Dispositive name
 * @param commandName Command Name
 * @param content Command to be stored
 * @return ESP_OK if was stored, else not stored.
 */
esp_err_t updateRFIDJSON(char *name, char *commandName, char *content);

/**
 * Method to populate Dropdown
 * @param indev Arguments to populate dropDwn
 */
void populateDropdownCommands(lv_event_t *indev);


#endif //RFIDDATASTORE_H
