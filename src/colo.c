/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : colo.c
 * Date    : 22 jun 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

/******************************************************************************
 * Header files
 *****************************************************************************/
#include "lcdc.h"
#include "lise.h"
#include <string.h>
#include "pomm.h"
#include "colo.h"
#include "butt.h"
#include "menu.h"
#include "src-gen/mstm.h"
#include "src-gen/sc_types.h"
#include "ledc.h"

typedef enum
{
    STATE_INIT_E,
    STATE_START_E,
    STATE_PREHEAT_E,
    STATE_CORRUPT_EEPROM_E,
    STATE_INIT_OK_E,
    STATE_CHOOSE_EXP_TYPE_E,
    STATE_SELECT_PCB_EXP_E,
    STATE_SELECT_SOLDER_MASK_EXP_E,
    STATE_SELECT_SOLDER_MASK_HARDENING_E,
    STATE_SELECT_CUSTOM_EXP_E,
    STATE_PLACE_ITEM_E,
    STATE_EXPOSURE_E,
    STATE_EXPOSING_E,
    STATE_SETT_PRE_HEAT_E,
    COLO_ST_NONE
} tSTATE_E;

typedef struct
{
    tSTATE_E current_E;
    tU32 timeInState_U32;
    tU08 internal;

} tSTATE_STR;

/******************************************************************************
 * Function prototypes
 *****************************************************************************/

Mstm handle;

/******************************************************************************
 * Functions
 *****************************************************************************/
void Colo_init(void)
{
    mstm_init(&handle);
    mstm_enter(&handle);
}

void Colo_loop(void)
{
    if (Butt_pressedFlank_B(BUTT_LEFT_E, COLO_T))
    {
        mstmIface_raise_leftButtonPressed(&handle);
    }
    if (Butt_pressedFlank_B(BUTT_RIGHT_E, COLO_T))
    {
        mstmIface_raise_rightButtonPressed(&handle);
    }
    if (Butt_getButtonState_E(BUTT_LEFT_E) == BUTT_RELEASED_E)
    {
        mstmIface_raise_leftButtonReleased(&handle);
    }

    if (FALSE)
    {
        mstmIface_raise_eepromDataVerifiedOk(&handle);
    }
    if (FALSE)
    {
        mstmIface_raise_eepromDataVerifiedCorrupt(&handle);
    }
    if (Pomm_powerDown_B() == TRUE)
    {
        mstmIface_raise_powerDown(&handle);
    }
    mstm_runCycle(&handle);
}
