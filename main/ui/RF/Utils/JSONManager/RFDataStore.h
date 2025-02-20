//
// Created by salman on 8/02/25.
//

#ifndef RFDATASTORE_H
#define RFDATASTORE_H

#define MAX_DEVICES 10
#define MAX_COMMANDS 4
#include <misc/lv_types.h>

static int rfDispCount = 0;


typedef struct {
    char name[32]; // Nombre del dispositivo
    char id[10];
    char type[16]; // Tipo de dispositivo
    char frequency; // Frecuencia seleccionada
    char command[MAX_COMMANDS][10];
} DeviceRF;


static DeviceRF devicesRf[MAX_DEVICES];

void init_spiffs();


void saveRfDispositives(lv_event_t *indev);
#endif //RFDATASTORE_H
