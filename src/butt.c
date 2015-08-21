/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : butt.c
 * Date    : 17 maj 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

#include "butt.h"
#include "gpio.h"
#include "types.h"
#include "util.h"

#define NOF_BUTTONS 2
#define BUTT_FILTER_TIME 30       /* [30 ms] */
#define BUTT_PRESS_TIME 100       /* [100 ms] */
#define BUTT_LONG_PRESS_TIME 5000 /* [5000 ms] */

//static tBUTT_STATE_E         butt_buttonFilterRaw[NOF_BUTTONS];
static tBUTT_STATE_EXTENDED_E butt_stateExtended_aE[NOF_BUTTONS];
static tBUTT_STATE_STR butt_state_astr[NOF_BUTTONS];

static void calcFilter(void);
static void calcState(void);

tBUTT_STATE_E Butt_getButtonState_E(const tBUTT_BUTTON_E button_E)
{
    return butt_state_astr[button_E].state_E;
}
tBUTT_STATE_EXTENDED_E Butt_getButtonStateExtended_E(
        const tBUTT_BUTTON_E button_E)
{
    return butt_stateExtended_aE[button_E];
}

void Butt_init(void)
{
    gpio_conf(LEFT_BUTTON_CFG, INPUT, PULLUP);
    gpio_conf(RIGHT_BUTTON_CFG, INPUT, PULLUP);
}

void Butt_loop(void)
{
    calcFilter();
    calcState();
}

static void calcFilter(void)
{
    static tBUTT_STATE_STR buttonRawOld_astr[NOF_BUTTONS];
    tBUTT_STATE_E buttonRaw_aE[NOF_BUTTONS];

    /* Invert the digital state and shift the bit to LSB position */
    buttonRaw_aE[BUTT_LEFT_E] = (
            gpio_tst(LEFT_BUTTON_CFG) ? BUTT_RELEASED_E : BUTT_PRESSED_E);
    buttonRaw_aE[BUTT_RIGHT_E] = (
            gpio_tst(RIGHT_BUTTON_CFG) ? BUTT_RELEASED_E : BUTT_PRESSED_E);

    tU08 i_U08;
    for (i_U08 = 0; i_U08 < NOF_BUTTONS; i_U08++)
    {
        /* Check if the raw button value is still the same. */
        if (buttonRawOld_astr[i_U08].state_E == buttonRaw_aE[i_U08])
        {
            /* Still the same value increment counter. */
            Util_safeUIntIncrement(&buttonRawOld_astr[i_U08].timeInState_U16);
        }
        else
        {
            /* Glitching value, reset counter. */
            buttonRawOld_astr[i_U08].timeInState_U16 = 0;
        }

        /* Check if the raw value is stable and different. */
        if ((buttonRawOld_astr[i_U08].timeInState_U16
                >= (BUTT_FILTER_TIME / BUTT_T))
                && (butt_state_astr[i_U08].state_E != buttonRaw_aE[i_U08]))
        {
            butt_state_astr[i_U08].state_E = buttonRaw_aE[i_U08];
            butt_state_astr[i_U08].timeInState_U16 = 0;
        }
        else
        {
            Util_safeUIntIncrement(&butt_state_astr[i_U08].timeInState_U16);
        }

        /* Remember value until next tick. */
        buttonRawOld_astr[i_U08].state_E = buttonRaw_aE[i_U08];
    }
}

static void calcState(void)
{
    /* Holds the time a button has been pressed. */
    static tU16 tiPressed_U16[NOF_BUTTONS];

    /* Loop through the buttons and update states of them. */
    tU08 i_U08;
    for (i_U08 = 0; i_U08 < NOF_BUTTONS; i_U08++)
    {
        if (butt_state_astr[i_U08].state_E == BUTT_PRESSED_E)
        {
            /* Button is pressed, count up the time the button has been pressed. */
            Util_safeUIntIncrement(&tiPressed_U16[i_U08]);
        }
        else
        {
            /* Reset counter */
            tiPressed_U16[i_U08] = 0;
        }

        switch (butt_stateExtended_aE[i_U08])
        {
        case BUTT_STATE_EXT_RELEASED:
            if ((tiPressed_U16[i_U08] >= BUTT_PRESS_TIME / BUTT_T)
                    && (butt_state_astr[i_U08].state_E == BUTT_PRESSED_E))
            {
                butt_stateExtended_aE[i_U08] = BUTT_STATE_EXT_PRESS_FLANK_E;
            }
            break;
        case BUTT_STATE_EXT_PRESS_FLANK_E:
            if (butt_state_astr[i_U08].state_E == BUTT_RELEASED_E)
            {
                butt_stateExtended_aE[i_U08] = BUTT_STATE_EXT_RELEASED_FLANK_E;
            }
            else
            {
                butt_stateExtended_aE[i_U08] = BUTT_STATE_EXT_PRESSED_E;
            }
            break;
        case BUTT_STATE_EXT_PRESSED_E:
            if (butt_state_astr[i_U08].state_E == BUTT_RELEASED_E)
            {
                butt_stateExtended_aE[i_U08] = BUTT_STATE_EXT_RELEASED_FLANK_E;
            }
            else
            {
                if (tiPressed_U16[i_U08] >= BUTT_LONG_PRESS_TIME / BUTT_T)
                {
                    butt_stateExtended_aE[i_U08] =
                            BUTT_STATE_EXT_LONG_PRESS_FLANK_E;
                }
            }
            break;
        case BUTT_STATE_EXT_LONG_PRESS_FLANK_E:
            if (butt_state_astr[i_U08].state_E == BUTT_RELEASED_E)
            {
                butt_stateExtended_aE[i_U08] = BUTT_STATE_EXT_RELEASED_FLANK_E;
            }
            else
            {
                butt_stateExtended_aE[i_U08] = BUTT_STATE_EXT_LONG_PRESSED_E;
            }
            break;
        case BUTT_STATE_EXT_LONG_PRESSED_E:
            if (butt_state_astr[i_U08].state_E == BUTT_RELEASED_E)
            {
                butt_stateExtended_aE[i_U08] = BUTT_STATE_EXT_RELEASED_FLANK_E;
            }
            break;
        case BUTT_STATE_EXT_RELEASED_FLANK_E:
            if ((butt_state_astr[i_U08].state_E == BUTT_PRESSED_E)
                    && (tiPressed_U16[i_U08] >= BUTT_PRESS_TIME / BUTT_T))
            {
                butt_stateExtended_aE[i_U08] = BUTT_STATE_EXT_PRESS_FLANK_E;
            }
            else
            {
                butt_stateExtended_aE[i_U08] = BUTT_RELEASED_E;
            }
            break;
        default:
            /* Shouln't end up here! */
            butt_stateExtended_aE[i_U08] = BUTT_RELEASED_E;
            break;
        }
    }
}

tB Butt_pressedFlank_B(tBUTT_BUTTON_E button_E, tU16 sampleTime_U16)
{
    tB buttonPressFlank_B = FALSE;

    if ((butt_state_astr[button_E].state_E == BUTT_PRESSED_E)
            && ((butt_state_astr[button_E].timeInState_U16 * BUTT_T)
                    < sampleTime_U16))
    {
        buttonPressFlank_B = TRUE;
    }

    return buttonPressFlank_B;
}
