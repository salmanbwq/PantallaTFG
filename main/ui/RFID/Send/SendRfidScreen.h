//
// Created by salman on 7/02/25.
//

#ifndef SENDRFIDSCREEN_H
#define SENDRFIDSCREEN_H

void goToSendRfidScreen(lv_event_t *event);

static void sendRfidScreen(void);

static void sendStoredCommadRFID(lv_event_t *event);

static void sendEspNowSendCommandRFID(char commandToSend[150]);

#endif //SENDRFIDSCREEN_H
