//
// Created by salman on 15/02/25.
//

#ifndef CONFIRMATIONPOPUP_H
#define CONFIRMATIONPOPUP_H
#include <misc/lv_types.h>

/**
 * Method to dispay a popUp
 * @param parent Screen object where confirmation popUp will be displayed.
 * @param message Message to be displayed.
 */
void showConfirmationPopup(lv_obj_t *parent, const char *message);

#endif //CONFIRMATIONPOPUP_H
