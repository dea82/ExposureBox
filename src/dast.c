/*
 * nvmm.c
 *
 *  Created on: 18 mar 2015
 *      Author: andreas
 */

#include "types.h"
#include "stat.h"
#include <avr/pgmspace.h>
#include "dast.h"
#include "eepr.h"
#include <util/crc16.h>

#define PRIMARY_MEM_ADDRESS 0x0
#define SECONDARY_MEM_ADDRESS AT25256_MEM_SIZE_BYTE/2

#define COUNTER_ADDRESS 0x0
#define TIMER_ADDRESS 0x100
#define SETTINGS_ADDRESS 0x200

typedef enum
{
    PRIMARY_E, SECONDARY_E
} tMemoryBlock_E;

typedef enum
{
    COUNTERS_E, TIMERS_E, SETTINGS_E
} tStorage_E;

typedef const struct
{
    tDast_Type_E type_E;
    tU16 offset_U16;
    void *ramMirror_p;
}PROGMEM tDataElement_str;

static void verifyEeprom(void);

typedef struct
{
    tU16 starts_U16;
    tU16 lightsActivation_U16;
    tU16 userAbsenceWarnings_U16;
    tU16 pcbExp_U16;
    tU16 solderMaskExp_U16;
    tU16 solderMaskHardening_U16;
    tU16 customExp_U16;
    tU16 crc_U16;
} tCounterElements_str;

typedef struct
{
    tU32 online_U32;
    tU32 lightsOn_U32;
    tU16 crc_U16;
} tTimerElements_str;

typedef struct
{
    tU16 pcbExpTime_U16;
    tU16 solderMaskExp_U16;
    tU16 solderMaskHardening_U16;
    tU16 customExpTime_U16;
    tU08 generalSettings1_U08;
    tU16 crc_U16;
} tSettingElements_str;

tCounterElements_str counterRamMirror_str;
tTimerElements_str timerRamMirror_str;
tSettingElements_str settingRamMirror_str;

tDast_verificationStatus_E startupCheckStatus_aE[] =
{
/* COUNTERS_E */DAST_UNKNOWN_E,
/* TIMERS_E   */DAST_UNKNOWN_E,
/* SETTINGS_E */DAST_UNKNOWN_E };

tDast_verificationSolution_E startupCheckSolution_aE[] =
{
/* COUNTERS_E */DAST_NONE_E,
/* TIMERS_E   */DAST_NONE_E,
/* SETTINGS_E */DAST_NONE_E };

tDataElement_str counterElementsDef_astr[] =
{
/*   NAME                           TYPE        ADDR  */
{ /* DAST_STARTS_E                */DAST_U16_E, 0x0,
        &counterRamMirror_str.starts_U16 },
{ /* DAST_LIGHTS_ACTIVATION_E     */DAST_U16_E, 0x2,
        &counterRamMirror_str.lightsActivation_U16 },
{ /* DAST_USER_ABSENCE_WARNINGS_E */DAST_U16_E, 0x4,
        &counterRamMirror_str.userAbsenceWarnings_U16 },
{ /* DAST_PCB_EXP_E               */DAST_U16_E, 0x6,
        &counterRamMirror_str.pcbExp_U16 },
{ /* DAST_SOLDER_MASK_EXP_E       */DAST_U16_E, 0x8,
        &counterRamMirror_str.solderMaskExp_U16 },
{ /* DAST_SOLDER_MASK_HARDENING_E */DAST_U16_E, 0xA,
        &counterRamMirror_str.solderMaskHardening_U16 },
{ /* DAST_CUSTOM_EXP_E            */DAST_U16_E, 0xC,
        &counterRamMirror_str.customExp_U16 },
{ /* CRC CRC CRC CRC CRC CRC CRC  */DAST_U16_E, 0xE,
        &counterRamMirror_str.crc_U16 } };

tDataElement_str timerElementsDef_astr[] =
{
/*   NAME             TYPE        ADDR  */
{ /* DAST_ONLINE_E   */DAST_U32_E, 0x0, &timerRamMirror_str.online_U32 },
{ /* DAST_TIMER_ON_E */DAST_U32_E, 0x4, &timerRamMirror_str.lightsOn_U32 },
{ /* CRC CRC CRC CRC */DAST_U16_E, 0x8, &timerRamMirror_str.crc_U16 } };

tDataElement_str settingElementsDef_astr[] =
{
/*   NAME                      TYPE        ADDR  */
{ /* DAST_PCB_EXP_TIME_E              */DAST_U16_E, 0x0,
        &settingRamMirror_str.pcbExpTime_U16 },
{ /* DAST_SOLDER_MASK_EXP_TIME_E      */DAST_U16_E, 0x2,
        &settingRamMirror_str.solderMaskExp_U16 },
{/* DAST_SOLDER_MASK_HARDENING_TIME_E */DAST_U16_E, 0x4,
        &settingRamMirror_str.solderMaskHardening_U16 },
{/* DAST_CUSTOM_EXP_TIME_E            */DAST_U16_E, 0x6,
        &settingRamMirror_str.customExpTime_U16 },
{/* DAST_GENERAL_SETTINGS1_E          */DAST_U08_E, 0x8,
        &settingRamMirror_str.generalSettings1_U08 },
{/* CRC CRC CRC CRC CRC CRC CRC CRC   */DAST_U16_E, 0x9,
        &settingRamMirror_str.crc_U16 } };

tDataElement_str *memoryBlocks_pastr[] =
{ counterElementsDef_astr, timerElementsDef_astr, settingElementsDef_astr };

void Dast_init(void)
{
    verifyEeprom();
}

static void verifyEeprom(void)
{
    for (tU08 i_U08 = 0; i_U08 < COUNT(memoryBlocks_pastr); i_U08++)
    {
        for (tU08 j_U08 = 0; i_U08 < COUNT(memoryBlocks_pastr); j_U08++)
        {

        }
    }
}
