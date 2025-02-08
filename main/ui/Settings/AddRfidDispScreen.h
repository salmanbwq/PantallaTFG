//
// Created by salman on 7/02/25.
//

#ifndef ADDRFIDDISPSCREEN_H
#define ADDRFIDDISPSCREEN_H+

#define MAX_DEVICES 10



typedef struct {
    char id[10];
    char name[32];
    char type[16];
    char command[10];
} DeviceRfid;


static DeviceRfid devicesRfid[MAX_DEVICES];

static void addrfiddispScreen();

void goToAddRfidScreen(lv_event_t *event);

static lv_obj_t *addRfidScreen;

#endif //ADDRFIDDISPSCREEN_H
