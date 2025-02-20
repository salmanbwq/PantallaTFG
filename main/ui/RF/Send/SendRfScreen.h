//
// Created by salman on 7/02/25.
//

#ifndef SENDRFSCREEN_H
#define SENDRFSCREEN_H
static lv_obj_t *rtrn;

static uint16_t selectedId;


static char *ID = "SendRfScreen";

void goToSendRfScreen(lv_event_t *event);

static void sendRfScreen(void);

#endif //SENDRFSCREEN_H
