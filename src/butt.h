/*
 * butt.h
 *
 *  Created on: 17 maj 2014
 *      Author: andreas
 */

#ifndef BUTT_H_
#define BUTT_H_

#include "types.h"

#define BUTT_T 10

typedef enum
{
    BUTT_LEFT_E = 0, BUTT_RIGHT_E = 1
} tBUTT_BUTTON_E;
typedef enum
{
    BUTT_RELEASED_E, BUTT_PRESSED_E
} tBUTT_STATE_E;

typedef struct
{
    tBUTT_STATE_E state_E;
    tU16 timeInState_U16;
} tBUTT_STATE_STR;

typedef enum
{
    BUTT_STATE_EXT_RELEASED,
    BUTT_STATE_EXT_PRESS_FLANK_E,
    BUTT_STATE_EXT_PRESSED_E,
    BUTT_STATE_EXT_LONG_PRESS_FLANK_E,
    BUTT_STATE_EXT_LONG_PRESSED_E,
    BUTT_STATE_EXT_RELEASED_FLANK_E
} tBUTT_STATE_EXTENDED_E;

void Butt_init(void);
void Butt_loop(void);
tBUTT_STATE_E Butt_getButtonState_E(const tBUTT_BUTTON_E button_E);
tBUTT_STATE_EXTENDED_E Butt_getButtonStateExtended_E(
        const tBUTT_BUTTON_E button_E);
tB Butt_pressedFlank_B(tBUTT_BUTTON_E button_E, tU16 sampleTime_U16);

#endif /* BUTT_H_ */
