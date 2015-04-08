/*
 * dast.h
 *
 *  Created on: 19 mar 2015
 *      Author: andreas
 */

#ifndef DAST_H_
#define DAST_H_

#include "stat.h"
#include "types.h"

typedef enum
{
    DAST_U08_E, DAST_U16_E, DAST_U32_E
} tDast_Type_E;

typedef enum
{
    DAST_STARTS_E,
    DAST_LIGHTS_ACTIVATION_E,
    DAST_USER_ABSENCE_WARNINGS_E,
    DAST_PCB_EXP_E,
    DAST_SOLDER_MASK_EXP_E,
    DAST_SOLDER_MASK_HARDENING_E,
    DAST_CUSTOM_EXP_E,
    DAST_NOF_COUNTERS_E
} tDast_Counters_E;

typedef enum
{
    DAST_ONLINE_E, DAST_LIGHTS_ON_E
} tDast_Timers_E;

typedef enum
{
    DAST_PCB_EXP_TIME_E,
    DAST_SOLDER_MASK_EXP_TIME_E,
    DAST_SOLDER_MASK_HARDENING_TIME_E,
    DAST_CUSTOM_EXP_TIME_E,
    DAST_GENERAL_SETTINGS1_E
} tDast_Settings_E;

typedef const struct
{
    tDast_Type_E type_E;
    tU16 offset_U16;
} tDast_BlockStructure_str;

typedef enum
{
    DAST_UNKNOWN_E, DAST_OK_E, DAST_CORRUPT_E
} tDast_verificationStatus_E;

typedef enum
{
    DAST_NONE_E, DAST_FIXED_E, DAST_LOST_E
} tDast_verificationSolution_E;

tDast_BlockStructure_str *Stat_getCounterStructure_pstr(void);
tDast_BlockStructure_str *Stat_getTimerStructure_pstr(void);

#endif /* DAST_H_ */
