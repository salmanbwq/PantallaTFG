//
// Created by salman on 16/02/25.
//

#ifndef LOADING_H
#define LOADING_H
#include <misc/lv_types.h>

/**
 * Method to display a loading popUp
 * @param parent Screen object where loading popUp will be displayed.
 */
void showLoadingPopup(lv_obj_t *parent);

/**
 * Method to change popUp status.
 * @param done Flag to change popUp status.
 */
void setLoadingDone(const bool done);

#endif //LOADING_H
