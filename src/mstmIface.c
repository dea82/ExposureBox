/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : mstmIface.c
 * Date    : 19 okt 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

/******************************************************************************
 * Header files
 *****************************************************************************/
#include "src-gen/sc_types.h"
#include "src-gen/mstm.h"
#include "menu.h"
#include "types.h"
#include "stmt.h"
#include "lcdc.h"


/******************************************************************************
 * Function prototypes
 *****************************************************************************/
/******************************************************************************
 * Functions
 *****************************************************************************/

void mstm_setTimer(Mstm* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic)
{
    Stmt_setTimer(handle, evid, time_ms, periodic, mstm_raiseTimeEvent);
}

void mstm_unsetTimer(Mstm* handle, const sc_eventid evid)
{
    Stmt_unsetTimer(evid);
}

sc_integer mstmIface_nofExpTypes(void)
{
    return (sc_integer)Menu_nofExpTypes();
}

void mstmIface_printStartupCheck(void)
{
    Menu_clear();
    Menu_printText(23, LCDC_LINE1, LCDC_CENTER);
}
void mstmIface_printStartUpCheckFailed(void)
{
    Menu_clear();
    Menu_printText(22, LCDC_LINE1, LCDC_CENTER);
}
void mstmIface_printStartUpCheckOk(void)
{
    Menu_clear();
    Menu_printText(2, LCDC_LINE1, LCDC_CENTER);
}
void mstmIface_playToneWarning(void)
{

}

void mstmIface_printStartScreen(void)
{
    Menu_clear();
    Menu_printText(0, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(6, MENU_BUTTON_LEFT_E);
    Menu_printButton(7, MENU_BUTTON_RIGHT_E);
}

void mstmIface_printSelectExpType(const sc_integer type)
{
    Menu_clear();
    Menu_printSelection(17, 0, type);
    Menu_printButton(12, MENU_BUTTON_LEFT_E);
    Menu_printButton(9, MENU_BUTTON_RIGHT_E);
}

void mstmIface_printExposure(const sc_integer type)
{
    Menu_clear();
    Menu_printText(13, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(6, MENU_BUTTON_LEFT_E);
    Menu_printButton(15, MENU_BUTTON_RIGHT_E);
    //TODO: Print timer
}

void mstmIface_printExposing(void)
{
    Menu_clear();
    Menu_printText(24, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(14, MENU_BUTTON_LEFT_E);
    Menu_printButton(15, MENU_BUTTON_RIGHT_E);
}
void mstmIface_printExposurePaused(void)
{
    Menu_clear();
    Menu_printText(25, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(26, MENU_BUTTON_LEFT_E);
    Menu_printButton(15, MENU_BUTTON_RIGHT_E);
}
void mstmIface_printExposureDone(void)
{

}

void mstmIface_printSetTimersDefaultValue(void)
{
    Menu_clear();
    Menu_printText(28, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(12, MENU_BUTTON_LEFT_E);
    Menu_printButton(9, MENU_BUTTON_RIGHT_E);
}

void mstmIface_writeCustomTimer(void)
{

}

void mstmIface_printStatistics(void)
{
    Menu_clear();
    Menu_printText(27, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(12, MENU_BUTTON_LEFT_E);
    Menu_printButton(29, MENU_BUTTON_RIGHT_E);
}

void mstmIface_printNofStarts(void)
{
    Menu_clear();
    Menu_printText(10, LCDC_LINE1, LCDC_LEFT);
    Menu_printButton(9, MENU_BUTTON_RIGHT_E);
}

void mstmIface_printClearStatQuestion(void)
{
    Menu_clear();
    Menu_printText(30, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(31, MENU_BUTTON_LEFT_E);
    Menu_printButton(32, MENU_BUTTON_RIGHT_E);
}

void mstmIface_printConfirmClear(void)
{
    Menu_clear();
    Menu_printText(33, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(34, MENU_BUTTON_LEFT_E);
    Menu_printButton(15, MENU_BUTTON_RIGHT_E);
}

void mstmIface_printClearDone(void)
{
    Menu_clear();
    Menu_printText(35, LCDC_LINE1, LCDC_CENTER);
    Menu_printButton(12, MENU_BUTTON_RIGHT_E);
}

void mstmIface_printClearing(void)
{
    Menu_clear();
    Menu_printText(36, LCDC_LINE1, LCDC_CENTER);
}
