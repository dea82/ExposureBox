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
    DAST_ONLINE_E, DAST_LIGHTS_ON_E, DAST_NOF_TIMERS_E
} tDast_Timers_E;

typedef enum
{
    DAST_PCB_EXP_TIME_E,
    DAST_SOLDER_MASK_EXP_TIME_E,
    DAST_SOLDER_MASK_HARDENING_TIME_E,
    DAST_CUSTOM_EXP_TIME_E,
    DAST_LIGHT_ON_CALIBRATION_E,
    DAST_LIGHT_OFF_CALIBRATION_E,
    DAST_GENERAL_SETTINGS1_E,
    DAST_NOF_SETTINGS_E
} tDast_Settings_E;

typedef enum
{
    DAST_UNKNOWN_E, DAST_OK_E, DAST_CORRUPT_E
} tDast_verificationStatus_E;

typedef enum
{
    DAST_NONE_E, DAST_FIXED_E, DAST_PARTIALLY_LOST_E, DAST_LOST_E, DAST_UNFIXED_E
} tDast_verificationSolution_E;

typedef struct
{
    tDast_verificationStatus_E status_E;
    tDast_verificationSolution_E solution_E;
} tDast_VerificationStatus_str;

typedef struct
{
    tDast_VerificationStatus_str primary_str;
    tDast_VerificationStatus_str secondary_str;
} tDast_MemoryBlockVerStat_str;

void Dast_init(void);
tDast_VerificationStatus_str Dast_getVerificationStatus_E(void);

tU16 Dast_getNofStarts_U16(void);
void Dast_clearStat(void);
void Dast_increaseNofStarts(void);

#endif /* DAST_H_ */
