//
// Created by salman on 8/02/25.
//

#ifndef RFDATASTORE_H
#define RFDATASTORE_H
#include <esp_err.h>
#include <cJSON/cJSON.h>
#include <misc/lv_types.h>

/**
 * Method to update RF Device information
 * @param name Device name
 * @param commandName Command name
 * @param content Command content
 * @return ESP_OK if device was updated, ESP_FAIL otherwise
 */
esp_err_t updateRFJSON(char *name, const char *commandName, char *content);

static cJSON *createGarageObj();

static cJSON *createAlarmObj();

static cJSON *getJson(const char *name, const char *type, const char *freq);

/**
 * Method to add an empty RF Device
 * @param indev Device Arguments
 */
void saveRfDispositives(lv_event_t *indev);
#endif //RFDATASTORE_H
