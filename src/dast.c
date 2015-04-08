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

typedef struct
{
    tDast_verificationStatus_E status_E;
    tDast_verificationSolution_E solution_E;
} tVerificationStatus_str;

tVerificationStatus_str verificationStatus_astr[] =
{
{/* COUNTERS   */DAST_UNKNOWN_E, DAST_NONE_E },
{/* TIMERS_E   */DAST_UNKNOWN_E, DAST_NONE_E },
{/* SETTINGS_E */DAST_UNKNOWN_E, DAST_NONE_E } };

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
typedef const struct
{
    tU16 address_U16;
    tDataElement_str *elements_str;
}PROGMEM tMemoryBlock_str;

tMemoryBlock_str memoryBlock_pastr[] =
{
{ COUNTER_ADDRESS, counterElementsDef_astr },
{ TIMER_ADDRESS, timerElementsDef_astr },
{ SETTINGS_ADDRESS, settingElementsDef_astr } };

static void verifyEeprom(void);
static tDast_verificationStatus_E readEepromToRam(
        tMemoryBlock_str memoryBlock_str, tU16 startAddress_U16);
static tU08 getNofBytes(tDast_Type_E type_E);
static void updateCrc(tU16 *crc_pU16, tU08 *data_pU08, tDast_Type_E type_E);
static tVerificationStatus_str verifyMemoryBlock(
        tMemoryBlock_str memoryBlock_str);

void Dast_init(void)
{
    verifyEeprom();
}

static void verifyEeprom(void)
{
    for (tU08 i_U08 = 0; i_U08 < COUNT(memoryBlock_pastr); i_U08++)
    {
        verificationStatus_astr[i_U08] = verifyMemoryBlock(
                memoryBlock_pastr[i_U08]);
    }
}

static tVerificationStatus_str verifyMemoryBlock(
        tMemoryBlock_str memoryBlock_str)
{
    /*
     * 1. Verify that PRI och SEC are equal, write to ram if they are, if not find out which is correct if any.
     * */
    tB syncronizedBlock = TRUE;
    tU16 primaryCrc_U16 = 0xFFFF;
    tU16 secondaryCrc_U16 = 0xFFFF;

    tU16 primaryAddress_U16;
    tU16 secondaryAddress_U16;

    for (tU08 i_U08 = 0; i_U08 < COUNT(memoryBlock_str.elements_str); i_U08++)
    {
        primaryAddress_U16 = PRIMARY_MEM_ADDRESS + memoryBlock_str.address_U16
                + memoryBlock_str.elements_str[i_U08].offset_U16;
        secondaryAddress_U16 = SECONDARY_MEM_ADDRESS
                + memoryBlock_str.address_U16
                + memoryBlock_str.elements_str[i_U08].offset_U16;
        if (i_U08 < (COUNT(memoryBlock_str.elements_str) - 1))
        {

            switch (memoryBlock_str.elements_str[i_U08].type_E)
            {
            case DAST_U08_E:
                if (Eepr_readByte_U08(primaryAddress_U16)
                        != Eepr_readByte_U08(secondaryAddress_U16))
                {
                    syncronizedBlock = FALSE;
                }
                break;
            case DAST_U16_E:
                if (Eepr_readInt_U16(primaryAddress_U16)
                        != Eepr_readInt_U16(secondaryAddress_U16))
                {
                    syncronizedBlock = FALSE;
                }
                break;
            case DAST_U32_E:
                if (Eepr_readLong_U32(primaryAddress_U16)
                        != Eepr_readLong_U32(secondaryAddress_U16))
                {
                    syncronizedBlock = FALSE;
                }
                break;
            }
        }
        else
        {

        }
    }
}

static tDast_verificationStatus_E readEepromToRam(
        tMemoryBlock_str memoryBlock_str, tU16 startAddress_U16)
{
    tU16 crc_U16 = 0xFFFF;
    tDast_verificationStatus_E verificationStatus_E = DAST_OK_E;

    for (tU08 i_U08 = 0; i_U08 < COUNT(memoryBlock_str.elements_str); i_U08++)
    {
        switch (memoryBlock_str.elements_str[i_U08].type_E)
        {
        case DAST_U08_E:
            *(tU08*) memoryBlock_str.elements_str[i_U08].ramMirror_p =
                    Eepr_readByte_U08(
                            startAddress_U16
                                    + memoryBlock_str.elements_str[i_U08].offset_U16);
            break;
        case DAST_U16_E:
            *(tU16*) memoryBlock_str.elements_str[i_U08].ramMirror_p =
                    Eepr_readInt_U16(
                            startAddress_U16
                                    + memoryBlock_str.elements_str[i_U08].offset_U16);
            break;
        case DAST_U32_E:
            *(tU32*) memoryBlock_str.elements_str[i_U08].ramMirror_p =
                    Eepr_readLong_U32(
                            startAddress_U16
                                    + memoryBlock_str.elements_str[i_U08].offset_U16);
            break;
        }

        /* Add to CRC if not field are last which is CRC field. */
        if (i_U08 < (COUNT(memoryBlock_str.elements_str) - 1))
        {
            updateCrc(&crc_U16,
                    (tU08*) memoryBlock_str.elements_str[i_U08].ramMirror_p,
                    memoryBlock_str.elements_str[i_U08].type_E);

        }
        else
        {
            /* Last element just added to ram mirror, this is the CRC field. Verify
             * that this value matches the computed. */
            if (*(tU16*) memoryBlock_str.elements_str[i_U08].ramMirror_p
                    == crc_U16)
            {
                verificationStatus_E = DAST_OK_E;
            }
            else
            {
                verificationStatus_E = DAST_CORRUPT_E;
            }
        }
    }
    return verificationStatus_E;
}

static tU08 getNofBytes(tDast_Type_E type_E)
{
    tU08 bytes_U08;

    switch (type_E)
    {
    case DAST_U08_E:
        bytes_U08 = 1;
        break;
    case DAST_U16_E:
        bytes_U08 = 2;
        break;
    case DAST_U32_E:
        bytes_U08 = 4;
        break;
    }
    return bytes_U08;
}

static void updateCrc(tU16 *crc_pU16, tU08 *data_pU08, tDast_Type_E type_E)
{
    tU08 bytes_U08 = getNofBytes(type_E);
    for (; bytes_U08 > 0; bytes_U08--)
    {
        *crc_pU16 = _crc16_update(*crc_pU16, *data_pU08);
    }
}
