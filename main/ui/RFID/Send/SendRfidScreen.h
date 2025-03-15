//
// Created by salman on 7/02/25.
//

#ifndef SENDRFIDSCREEN_H
#define SENDRFIDSCREEN_H

/**
 * Method to call Send RFID screen.
 */
void goToSendRfidScreen();

static void textAreaHandlerRFID(lv_event_t *e);

static void createKeyboardRFID(lv_obj_t *parent);

static void onSwitchChange(lv_event_t *event);

static void sendRfidScreen(void);

static void sendStoredCommadRFID();

static void onRFIDSendAction();

static void sendEspNowSendCommandRFID(char commandToSend[150]);

#endif //SENDRFIDSCREEN_H
