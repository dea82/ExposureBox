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

static void readCountersToRam(void);
static void readEepromToRam(tU16 eepromAddress_U16, void *ramMirror_p,
        tDast_Type_E type_E);

typedef const struct
{
    tDast_Type_E type_E;
    tU16 offset_U16;
    void *ramMirror_p;
} tDataElement_str;

typedef struct
{
    tU16 starts_U16;
    tU16 lightsActivation_U16;
    tU16 userAbsenceWarnings_U16;
    tU16 pcbExp_U16;
    tU16 solderMaskExp_U16;
    tU16 solderMaskHardening_U16;
    tU16 customExp_U16;
} tCounterElements_str;

tCounterElements_str counterRamMirror_str;

const tDataElement_str counterElementsDef_astr[] =
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
{ /* DAST_CUSTOM_EXP_TIME_E       */DAST_U16_E, 0xC,
        &counterRamMirror_str.customExp_U16 } };

void Dast_init(void)
{
    readCountersToRam();
}

static void readCountersToRam(void)
{
    for (tU08 i_U08 = 0; i_U08 < DAST_NOF_COUNTERS_E; i_U08++)
    {
        readEepromToRam(
        COUNTER_ADDRESS + counterElementsDef_astr[i_U08].offset_U16,
                counterElementsDef_astr[i_U08].ramMirror_p,
                counterElementsDef_astr[i_U08].type_E);

    }
}

static void readEepromToRam(tU16 eepromAddress_U16, void *ramMirror_p,
        tDast_Type_E type_E)
{
    switch (type_E)
    {
    case DAST_U16_E:
        *(tU16*)ramMirror_p = Eepr_readInt_U16(eepromAddress_U16);
        break;
    }
}
