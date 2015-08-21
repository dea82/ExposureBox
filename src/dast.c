/*
 * nvmm.c
 *
 *  Created on: 18 mar 2015
 *      Author: andreas
 */

/**
 * This module handles the data storage routines. It will verify the data before
 * everything is mirrored to RAM.
 *
 * Data storage memory (EEPROM) is divided into duplicated sections which all have their own CRC.
 * At startup each sections is verified primary against secondary and their CRC is verified.
 *
 */

#include "stdio.h"
#include "types.h"
#include "stat.h"
#include <avr/pgmspace.h>
#include "dast.h"
#include "eepr.h"
#include <util/crc16.h>
#include "seri.h"

#define DEBUG

#define PRIMARY_MEM_ADDRESS 0x0
#define SECONDARY_MEM_ADDRESS AT25256_MEM_SIZE_BYTE/2

#define COUNTER_ADDRESS 0x0
#define TIMER_ADDRESS 0x100
#define SETTINGS_ADDRESS 0x200

#define BYTES_MAX 4

typedef enum
{
    COUNTERS_E, TIMERS_E, SETTINGS_E
} tStorageType_E;

typedef const struct
{
    tU08 bytes_U08;
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
    tU16 solderMaskExpTime_U16;
    tU16 solderMaskHardeningTime_U16;
    tU16 customExpTime_U16;
    tU16 lightOnCalibration_U16;
    tU16 lightOffCalibration_U16;
    tU08 generalSettings1_U08;
    tU16 crc_U16;
} tSettingElements_str;

tVerificationStatus_str verificationStatus_astr[] =
{
{/* COUNTERS_E */DAST_UNKNOWN_E, DAST_NONE_E },
{/* TIMERS_E   */DAST_UNKNOWN_E, DAST_NONE_E },
{/* SETTINGS_E */DAST_UNKNOWN_E, DAST_NONE_E } };

tCounterElements_str counterRamMirror_str;
tTimerElements_str timerRamMirror_str;
tSettingElements_str settingRamMirror_str;

tDataElement_str counterElementsDef_astr[DAST_NOF_COUNTERS_E + 1] =
{
        /*   NAME                            BYTES ADDR RAMMIRROR */
        { /* DAST_STARTS_E                    */2, 0x0,
                &counterRamMirror_str.starts_U16 },
        { /* DAST_LIGHTS_ACTIVATION_E         */2, 0x2,
                &counterRamMirror_str.lightsActivation_U16 },
        { /* DAST_USER_ABSENCE_WARNINGS_E     */2, 0x4,
                &counterRamMirror_str.userAbsenceWarnings_U16 },
        { /* DAST_PCB_EXP_E                   */2, 0x6,
                &counterRamMirror_str.pcbExp_U16 },
        { /* DAST_SOLDER_MASK_EXP_E           */2, 0x8,
                &counterRamMirror_str.solderMaskExp_U16 },
        { /* DAST_SOLDER_MASK_HARDENING_E     */2, 0xA,
                &counterRamMirror_str.solderMaskHardening_U16 },
        { /* DAST_CUSTOM_EXP_E                */2, 0xC,
                &counterRamMirror_str.customExp_U16 },
        { /* CRC CRC CRC CRC CRC CRC CRC      */2, 0xE,
                &counterRamMirror_str.crc_U16 } };

tDataElement_str timerElementsDef_astr[DAST_NOF_TIMERS_E + 1] =
{
        /*   NAME                            BYTES ADDR RAMMIRROR */
        { /* DAST_ONLINE_E                    */4, 0x0,
                &timerRamMirror_str.online_U32 },
        { /* DAST_TIMER_ON_E                  */4, 0x4,
                &timerRamMirror_str.lightsOn_U32 },
        { /* CRC CRC CRC CRC                  */2, 0x8,
                &timerRamMirror_str.crc_U16 } };

tDataElement_str settingElementsDef_astr[DAST_NOF_SETTINGS_E + 1] =
        {
        /*   NAME                            BYTES ADDR RAMMIRROR */
        { /* DAST_PCB_EXP_TIME_E              */2, 0x0,
                &settingRamMirror_str.pcbExpTime_U16 },
        { /* DAST_SOLDER_MASK_EXP_TIME_E      */2, 0x2,
                &settingRamMirror_str.solderMaskExpTime_U16 },
        {/* DAST_SOLDER_MASK_HARDENING_TIME_E */2, 0x4,
                &settingRamMirror_str.solderMaskHardeningTime_U16 },
        {/* DAST_CUSTOM_EXP_TIME_E            */2, 0x6,
                &settingRamMirror_str.customExpTime_U16 },
        {/* DAST_LIGHT_ON_CALIBRATION_E       */2, 0x8,
                &settingRamMirror_str.lightOnCalibration_U16 },
        {/* DAST_LIGHT_OFF_CALIBRATION_E      */2, 0xA,
                &settingRamMirror_str.lightOffCalibration_U16 },
        {/* DAST_GENERAL_SETTINGS1_E          */1, 0xC,
                &settingRamMirror_str.generalSettings1_U08 },
        {/* CRC CRC CRC CRC CRC CRC CRC CRC   */2, 0xD,
                &settingRamMirror_str.crc_U16 } };

typedef const struct
{
    tU16 address_U16;
    tDataElement_str *elements_str;
    tU08 nofElements_U08;
} tMemoryBlock_str;

tMemoryBlock_str memoryBlock_pastr[] =
{
{ COUNTER_ADDRESS, counterElementsDef_astr, DAST_NOF_COUNTERS_E + 1 },
{ TIMER_ADDRESS, timerElementsDef_astr, DAST_NOF_TIMERS_E + 1 },
{ SETTINGS_ADDRESS, settingElementsDef_astr, DAST_NOF_SETTINGS_E + 1 } };

static void verifyEeprom(void);
static void updateCrc(tU16 *crc_pU16, tU08 *data_pU08, tU08 bytes_U08);
static tVerificationStatus_str verifyMemoryBlock(
        tMemoryBlock_str memoryBlock_str);
static tDast_verificationSolution_E clearMemoryBlock_E(
        tMemoryBlock_str memoryBlock_str);
static tB syncronizedData_B(tU08 *priData_pU08, tU08 *secData_pU08,
        tU08 bytes_U08);
static tB verifyCrc(tU08 priData_pU08[], tU16 crc_U16);

void Dast_init(void)
{
#ifdef DEBUG
    Seri_writeString("Verify EEPROM\n\n");
#endif
    verifyEeprom();
}

static void verifyEeprom(void)
{
    for (tU08 i_U08 = 0; i_U08 < COUNT(memoryBlock_pastr); i_U08++)
    {
        Seri_writeString("Verify block\n");
        verificationStatus_astr[i_U08] = verifyMemoryBlock(
                memoryBlock_pastr[i_U08]);
    }
}

static tVerificationStatus_str verifyMemoryBlock(
        tMemoryBlock_str memoryBlock_str)
{
    //char text[20];

    tB syncronizedBlock_B = TRUE;

    tVerificationStatus_str verificationStatus_str =
    { DAST_UNKNOWN_E, DAST_NONE_E };

    tU16 priCrc_U16 = 0xFFFF;
    tU16 secCrc_U16 = 0xFFFF;

    tU08 priDataBuffer_paU08[BYTES_MAX];
    tU08 secDataBuffer_paU08[BYTES_MAX];

    for (tU08 i_U08 = 0; i_U08 < memoryBlock_str.nofElements_U08; i_U08++)
    {
        Seri_writeString("Element\n");
        Eepr_read(priDataBuffer_paU08,
                PRIMARY_MEM_ADDRESS + memoryBlock_str.address_U16
                        + memoryBlock_str.elements_str[i_U08].offset_U16,
                memoryBlock_str.elements_str[i_U08].bytes_U08);
        Eepr_read(secDataBuffer_paU08,
                SECONDARY_MEM_ADDRESS + memoryBlock_str.address_U16
                        + memoryBlock_str.elements_str[i_U08].offset_U16,
                memoryBlock_str.elements_str[i_U08].bytes_U08);

        syncronizedBlock_B &= syncronizedData_B(priDataBuffer_paU08,
                secDataBuffer_paU08,
                memoryBlock_str.elements_str[i_U08].bytes_U08);

        if (i_U08 < memoryBlock_str.nofElements_U08 - 1)
        {
            /* Update CRC */
            Seri_writeString("Update CRC\n");
            updateCrc(&priCrc_U16, priDataBuffer_paU08,
                    memoryBlock_str.elements_str[i_U08].bytes_U08);
            updateCrc(&secCrc_U16, secDataBuffer_paU08,
                    memoryBlock_str.elements_str[i_U08].bytes_U08);
        }
        else
        {
            /* CRC */
            if (verifyCrc(priDataBuffer_paU08, priCrc_U16)
                    && verifyCrc(secDataBuffer_paU08, secCrc_U16)
                    && syncronizedBlock_B)
            {
                Seri_writeString("OK\n");
                verificationStatus_str.status_E = DAST_OK_E;
                verificationStatus_str.solution_E = DAST_NONE_E;
            }
            else if (verifyCrc(priDataBuffer_paU08, priCrc_U16) == FALSE)
            {
                Seri_writeString("Primary CRC not ok\n");
                verificationStatus_str.status_E = DAST_CORRUPT_E;
                verificationStatus_str.solution_E = clearMemoryBlock_E(
                        memoryBlock_str);

            }
            else if (verifyCrc(secDataBuffer_paU08, secCrc_U16) == FALSE)
            {
                Seri_writeString("Secondary CRC not ok\n");
                verificationStatus_str.status_E = DAST_CORRUPT_E;
            }
            else
            {
                Seri_writeString("Not syncro\n");
                verificationStatus_str.status_E = DAST_CORRUPT_E;
            }
        }
    }
    return verificationStatus_str;
}

static tDast_verificationSolution_E clearMemoryBlock_E(
        tMemoryBlock_str memoryBlock_str)
{

    tDast_verificationSolution_E solution_E;
    tU16 crc_U16 = 0xFFFF;
    tB cleared_B = TRUE;
    tU08 buffer_aU08[BYTES_MAX] =
    { 0, 0, 0, 0 };

    for (tU08 i_U08 = 0; i_U08 < memoryBlock_str.nofElements_U08; i_U08++)
    {

        if (i_U08 < (memoryBlock_str.nofElements_U08 - 1))
        {
            updateCrc(&crc_U16, buffer_aU08,
                    memoryBlock_str.elements_str[i_U08].bytes_U08);
            cleared_B &= Eepr_write_B(buffer_aU08,
                    PRIMARY_MEM_ADDRESS + memoryBlock_str.address_U16
                            + memoryBlock_str.elements_str[i_U08].offset_U16,
                    memoryBlock_str.elements_str[i_U08].bytes_U08);
            cleared_B &= Eepr_write_B(buffer_aU08,
                    SECONDARY_MEM_ADDRESS + memoryBlock_str.address_U16
                            + memoryBlock_str.elements_str[i_U08].offset_U16,
                    memoryBlock_str.elements_str[i_U08].bytes_U08);
            Seri_writeString("Clear\n");
            if (cleared_B == TRUE)
            {
                Seri_writeString("Cleared ok\n");
            }
            else
            {
                Seri_writeString("Cleared Nok\n");
            }
        }
        else
        {
            Seri_writeString("Update CRC\n");
            cleared_B &= Eepr_write_B((tU08*) &crc_U16,
                    PRIMARY_MEM_ADDRESS + memoryBlock_str.address_U16
                            + memoryBlock_str.elements_str[i_U08].offset_U16,
                    memoryBlock_str.elements_str[i_U08].bytes_U08);
            cleared_B &= Eepr_write_B((tU08*) &crc_U16,
                    SECONDARY_MEM_ADDRESS + memoryBlock_str.address_U16
                            + memoryBlock_str.elements_str[i_U08].offset_U16,
                    memoryBlock_str.elements_str[i_U08].bytes_U08);
            if (cleared_B == TRUE)
            {
                Seri_writeString("CRC written ok\n");
            }
            else
            {
                Seri_writeString("CRC not written ok\n");
            }

        }

    }

    if (cleared_B == TRUE)
    {
        solution_E = DAST_LOST_E;
    }
    else
    {
        solution_E = DAST_UNFIXED_E;
    }

    return solution_E;
}

static tB verifyCrc(tU08 priData_pU08[], tU16 crc_U16)
{
    tB ret_B = TRUE;
    for (tU08 i_U08 = 0; i_U08 < sizeof(crc_U16); i_U08++)
    {
        ret_B &= (priData_pU08[i_U08] == ((tU08*) &crc_U16)[i_U08]);
    }
    return ret_B;
}

static tB syncronizedData_B(tU08 *priData_pU08, tU08 *secData_pU08,
        tU08 bytes_U08)
{
    tB ret_B = TRUE;
    while (bytes_U08--)
    {
        ret_B &= (*priData_pU08++ == *secData_pU08++);
    }
    return ret_B;
}

static void updateCrc(tU16 *crc_pU16, tU08 *data_pU08, tU08 bytes_U08)
{
    for (; bytes_U08 > 0; bytes_U08--)
    {
        *crc_pU16 = _crc16_update(*crc_pU16, *data_pU08);
    }
}

tVerificationStatus_str Dast_getVerificationStatus_E(void)
{
    tVerificationStatus_str verificationStatus_str =
    { DAST_OK_E, DAST_NONE_E };
    for (tU08 i_U08 = 0; i_U08 < COUNT(verificationStatus_astr); i_U08++)
    {
        if (verificationStatus_astr[i_U08].status_E == DAST_CORRUPT_E)
        {
            verificationStatus_str.status_E = DAST_CORRUPT_E;
        }
    }

    return verificationStatus_str;
}
