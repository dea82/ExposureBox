/*
 * menu.h
 *
 *  Created on: 27 jun 2014
 *      Author: andreas
 */

#ifndef MENU_H_
#define MENU_H_

#include "types.h"
#include "lcdc.h"

typedef enum {MENU_BUTTON_LEFT_E, MENU_BUTTON_RIGHT_E} tMENU_BUTTON_E;

typedef struct{
    tU08 nofItems_U08;
    tU08 *values;
} tMENU_SELECTABLE_ITEM_STR;

void Menu_printText(const tU08 id, const tLine line, const tAlignment alignment);
void Menu_printButton(const tU08 id_U08, tMENU_BUTTON_E button_E);
void Menu_printTimer(void);
void Menu_clear(void);
tU08 Menu_nofExpTypes(void);
void Menu_printSelection(const tU08 textId_U08, const tU08 itemId_U08, const tU08 itemNo_U08);

#endif /* MENU_H_ */
