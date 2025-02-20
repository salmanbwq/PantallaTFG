//
// Created by salman on 7/02/25.
//

#ifndef SENDIRSCREEN_H
#define SENDIRSCREEN_H

static char *ID = "Send IR";

static lv_obj_t *sendIrScrn;

static lv_obj_t *rtrn;
static lv_obj_t *dropdwnHndlr;

static uint16_t selectedId;

void goToIrSend(lv_indev_t *indev);

static void sendIrScreen();


#endif //SENDIRSCREEN_H
