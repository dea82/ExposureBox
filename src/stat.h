/*
 * stat.h
 *
 *  Created on: 18 mar 2015
 *      Author: andreas
 */

#ifndef STAT_H_
#define STAT_H_
#include "types.h"

typedef enum
{
    STAT_STARTS_E,
    STAT_LIGHTS_ACTIVATION_E,
    STAT_USER_ABSENCE_WARNINGS_E,
    STAT_PCB_EXP_E,
    STAT_SOLDER_MASK_EXP_E,
    STAT_SOLDER_MASK_HARDENING_E,
    STAT_CUSTOM_EXP_E
} tStat_Counters_E;

typedef struct
{
    tU16 starts_U16;
    tU16 lightsActivation_U16;
    tU16 userAbsenceWarnings_U16;
    tU16 pcbExp_U16;
    tU16 solderMaskExp_U16;
    tU16 solderMaskHardening_U16;
    tU16 customExp_U16;
} tStat_Counters_str;

typedef struct
{
    tU32 online_U32;
    tU32 lightsOn_U32;
} tStat_Timers_str;

void Stat_increaseNofStart(void);
tU16 Stat_getNofStarts_U16(void);
void Stat_clearStat(void);
#endif /* STAT_H_ */
