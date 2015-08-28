/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : menu.c
 * Date    : 27 jun 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

/******************************************************************************
 * Header files
 *****************************************************************************/
#include <avr/pgmspace.h>
#include "lcdc.h"
#include "menu.h"
#include "util.h"
#include <string.h>
#include <stdio.h>

#define NOF_EXP_TYPES 4
/******************************************************************************
 * Function prototypes
 *****************************************************************************/
static void printText(const tU08 id, const tLine line, const tAlignment alignment);

/******************************************************************************
 * Functions
 *****************************************************************************/
const char PROGMEM string_0[]  = "UV Box";
const char PROGMEM string_1[]  = "Pre-heat?"; //Not used!
const char PROGMEM string_2[]  = "Startup check ok!";
const char PROGMEM string_3[]  = "PCB";
const char PROGMEM string_4[]  = "Solder mask";
const char PROGMEM string_5[]  = "Hardening";
const char PROGMEM string_6[]  = "Start";
const char PROGMEM string_7[]  = "Settings";
const char PROGMEM string_8[]  = "Select";
const char PROGMEM string_9[]  = "Next";
const char PROGMEM string_10[] = "Starts";
const char PROGMEM string_11[] = "Place item";
const char PROGMEM string_12[] = "OK";
const char PROGMEM string_13[] = "Exposure";
const char PROGMEM string_14[] = "Pause";
const char PROGMEM string_15[] = "Abort";
const char PROGMEM string_16[] = "Exposure done!";
const char PROGMEM string_17[] = "Type";
const char PROGMEM string_18[] = "PCB";
const char PROGMEM string_19[] = "Solder mask";
const char PROGMEM string_20[] = "Hardening";
const char PROGMEM string_21[] = "Custom";
const char PROGMEM string_22[] = "Startup check failed";
const char PROGMEM string_23[] = "Startup check...";
const char PROGMEM string_24[] = "Exposing";
const char PROGMEM string_25[] = "Exposure paused";
const char PROGMEM string_26[] = "Resume";
const char PROGMEM string_27[] = "Statistics";
const char PROGMEM string_28[] = "Set timers default";
const char PROGMEM string_29[] = "Exit";
const char PROGMEM string_30[] = "Erase statistics?";
const char PROGMEM string_31[] = "Yes";
const char PROGMEM string_32[] = "No";
const char PROGMEM string_33[] = "Press erase for 5 s!";
const char PROGMEM string_34[] = "Erase!";
const char PROGMEM string_35[] = "Statistics erased!";
const char PROGMEM string_36[] = "Clearing...";

PGM_P const string_table[] PROGMEM =
{
    string_0,
    string_1,
    string_2,
    string_3,
    string_4,
    string_5,
    string_6,
    string_7,
    string_8,
    string_9,
    string_10,
    string_11,
    string_12,
    string_13,
    string_14,
    string_15,
    string_16,
    string_17,
    string_18,
    string_19,
    string_20,
    string_21,
    string_22,
    string_23,
    string_24,
    string_25,
    string_26,
    string_27,
    string_28,
    string_29,
    string_30,
    string_31,
    string_32,
    string_33,
    string_34,
    string_35,
    string_36,
};

static tU08 exposureItems[NOF_EXP_TYPES] = {3, 4, 5, 21};
static tU08 onOff[2] = {0, 1};

static tMENU_SELECTABLE_ITEM_STR selectableItems[] = {
        {NOF_EXP_TYPES, exposureItems},
        {2, onOff}
};

static void printText(const tU08 id, const tLine line, const tAlignment alignment)
{
    char buffer[21] = "";
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[id])));
    Lcdc_writeStringAligned(buffer, line, alignment);
}

void Menu_clear(void)
{
    Lcdc_clear();
}

void Menu_printText(const tU08 id, const tLine line, const tAlignment alignment)
{
    printText(id, line, alignment);
}

void Menu_printButton(const tU08 id_U08, tMENU_BUTTON_E button_E)
{
    switch(button_E)
    {
    case MENU_BUTTON_LEFT_E:
        printText(id_U08, LCDC_LINE2, LCDC_LEFT);
        break;
    case MENU_BUTTON_RIGHT_E:
        printText(id_U08, LCDC_LINE2, LCDC_RIGHT);
        break;
    default:
        break;
    }
}

void Menu_printSelection(const tU08 textId_U08, const tU08 itemId_U08, const tU08 itemNo_U08)
{
    char buffer[21] = "";
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[textId_U08])));

    Util_concatString(buffer, ": ");
    strcpy_P(buffer + strlen(buffer), (PGM_P)pgm_read_word(&(string_table[selectableItems[itemId_U08].values[itemNo_U08]])));

    Lcdc_writeStringAligned(buffer, LCDC_LINE1, LCDC_LEFT);
}

void Menu_printCounterStat(const tU08 textId_U08, const tU16 counter_U16, const tLine line, const tAlignment alignment)
{
    char buffer[21] = "";
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[textId_U08])));

    sprintf(buffer + strlen(buffer), ": %i", counter_U16);
    Lcdc_writeStringAligned(buffer, line, alignment);

}

void Menu_printTimer(void)
{

}

tU08 Menu_nofExpTypes(void)
{
    return NOF_EXP_TYPES;
}
