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

typedef enum
{
    PRIMARY_E, SECONDARY_E
} tSector_E;

typedef enum
{
    PRIMARY_TO_SECONDARY_E, SECONDARY_TO_PRIMARY_E
} tCopyDirection_E;

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

tDast_VerificationStatus_str verificationStatus_astr[] =
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

//TODO: Borde utökas med en pekar till verifieringsstatus.
tMemoryBlock_str memoryBlock_pastr[] =
{
{ COUNTER_ADDRESS, counterElementsDef_astr, DAST_NOF_COUNTERS_E + 1 },
{ TIMER_ADDRESS, timerElementsDef_astr, DAST_NOF_TIMERS_E + 1 },
{ SETTINGS_ADDRESS, settingElementsDef_astr, DAST_NOF_SETTINGS_E + 1 } };

static void verifyEeprom(void);
static void updateCrc(tU16 *crc_pU16, void *data_p, tU08 bytes_U08);
static tDast_VerificationStatus_str verifyMemoryBlock_str(
        tMemoryBlock_str memoryBlock_str);
static tDast_verificationStatus_E verifyMemoryBlockInSector_E(
        tMemoryBlock_str memoryBlock_str, tSector_E sector_E);
static tDast_verificationSolution_E clearMemoryBlock_E(
        tMemoryBlock_str memoryBlock_str);

static tU16 getSectorStartAddress_U16(tSector_E sector_E);
static tB readMemoryBlockToRam(tMemoryBlock_str memoryBlock_str,
        tSector_E sector_E);
static tB ramMirrorConsistent_B(tMemoryBlock_str memoryBlock_str);
static tU16 calcCrcOfRamMirror_U16(tMemoryBlock_str memoryBlock_str);
static tB writeRamMirrorToSector_B(tMemoryBlock_str memoryBlock_str,
        tSector_E sector_E);
static tDast_verificationStatus_E compareRamMirrorWithSector_E(
        tMemoryBlock_str memoryBlock_str, tSector_E sector_E);
static tB dataEqual_B(void *priData_p, void *secData_p, tU08 bytes_U08);
static void clearRamMirror(tMemoryBlock_str memoryBlock_str);
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
        Seri_writeString("Verify block\n\n");
        verificationStatus_astr[i_U08] = verifyMemoryBlock_str(
                memoryBlock_pastr[i_U08]);
    }
}

static tDast_VerificationStatus_str verifyMemoryBlock_str(
        tMemoryBlock_str memoryBlock_str)
{
    tDast_VerificationStatus_str memoryBlockStatus_str;

    tDast_verificationStatus_E primarySectorVerificationStatus_E =
            verifyMemoryBlockInSector_E(memoryBlock_str, PRIMARY_E);

    /* Check if primary sector is healthy. */
    if (primarySectorVerificationStatus_E == DAST_OK_E)
    {
        Seri_writeString("Primary healthy\n");
        /* Primary sector healthy! */
        if (compareRamMirrorWithSector_E(memoryBlock_str, SECONDARY_E)
                == DAST_CORRUPT_E)
        {
            Seri_writeString("Secondary corrupt\n");
            /* Secondary is not up to date with primary, it must be corrupt! */
            memoryBlockStatus_str.status_E = DAST_CORRUPT_E;

            /* Try to write ram mirror to secondary sector! */
            if (writeRamMirrorToSector_B(memoryBlock_str, SECONDARY_E) == TRUE)
            {
                memoryBlockStatus_str.solution_E = DAST_FIXED_E;
            }
            else
            {
                memoryBlockStatus_str.solution_E = DAST_UNFIXED_E;
            }
        }
        else
        {
            Seri_writeString("Secondary healthy\n");
            /* Secondary also seems healthy or unknown. */
            memoryBlockStatus_str.status_E = DAST_OK_E;
            memoryBlockStatus_str.solution_E = DAST_NONE_E;

        }
    }
    else if (primarySectorVerificationStatus_E == DAST_CORRUPT_E)
    {
        Seri_writeString("Primary corrupt\n");
        /* Primary sector corrupt! Try with secondary. */
        memoryBlockStatus_str.status_E = DAST_CORRUPT_E;
        tDast_verificationStatus_E secondarySectorVerificationStatus_E =
                verifyMemoryBlockInSector_E(memoryBlock_str, SECONDARY_E);

        if (secondarySectorVerificationStatus_E == DAST_OK_E)
        {
            if (writeRamMirrorToSector_B(memoryBlock_str, PRIMARY_E))
            {
                memoryBlockStatus_str.solution_E = DAST_PARTIALLY_LOST_E;
            }
            else
            {
                memoryBlockStatus_str.solution_E = DAST_UNFIXED_E;
            }
        }
        else if (secondarySectorVerificationStatus_E == DAST_CORRUPT_E)
        {
            Seri_writeString("Secondary corrupt");
            memoryBlockStatus_str.solution_E = DAST_LOST_E;
            /* Both sectors are corrupt! Clear them both! */
            clearMemoryBlock_E(memoryBlock_str);
        }
    }
    else
    {
        /* Primary sector unknown. Don't try with secondary and use default values! */
        memoryBlockStatus_str.status_E = DAST_UNKNOWN_E;
        memoryBlockStatus_str.solution_E = DAST_NONE_E;

    }

    return memoryBlockStatus_str;

}

static tDast_verificationStatus_E compareRamMirrorWithSector_E(
        tMemoryBlock_str memoryBlock_str, tSector_E sector_E)
{

    tU16 sectorStartAddress_U16 = getSectorStartAddress_U16(sector_E);

    tU08 buffer_aU08[BYTES_MAX];

    tDast_verificationStatus_E verificationStatus_E = DAST_OK_E;

    for (tU08 i_U08 = 0; i_U08 < memoryBlock_str.nofElements_U08; i_U08++)
    {
        //TODO: If read is not possible!?
        Eepr_read(buffer_aU08,
                sectorStartAddress_U16 + memoryBlock_str.address_U16
                        + memoryBlock_str.elements_str[i_U08].offset_U16,
                memoryBlock_str.elements_str[i_U08].bytes_U08);
        if (!dataEqual_B(buffer_aU08,
                memoryBlock_str.elements_str[i_U08].ramMirror_p,
                memoryBlock_str.elements_str[i_U08].bytes_U08))
        {
            verificationStatus_E = DAST_CORRUPT_E;
        }
    }
    return verificationStatus_E;
}

static tB dataEqual_B(void *priData_p, void *secData_p, tU08 bytes_U08)
{
    tB ret_B = TRUE;
    while (bytes_U08--)
    {
        ret_B &= (*(tU08*) priData_p++ == *(tU08*) secData_p++);
    }
    return ret_B;
}

static tB writeRamMirrorToSector_B(tMemoryBlock_str memoryBlock_str,
        tSector_E sector_E)
{
    tB ret_B = TRUE;
    for (tU08 i_U08 = 0; i_U08 < memoryBlock_str.nofElements_U08; i_U08++)
    {
        ret_B &= Eepr_write_B(memoryBlock_str.elements_str[i_U08].ramMirror_p,
                getSectorStartAddress_U16(sector_E)
                        + memoryBlock_str.address_U16
                        + memoryBlock_str.elements_str[i_U08].offset_U16,
                memoryBlock_str.elements_str[i_U08].bytes_U08);
    }

    return ret_B;
}

static tDast_verificationStatus_E verifyMemoryBlockInSector_E(
        tMemoryBlock_str memoryBlock_str, tSector_E sector_E)
{
    tDast_verificationStatus_E verificationStatus_E;
    if (readMemoryBlockToRam(memoryBlock_str, sector_E) == TRUE)
    {
        if (ramMirrorConsistent_B(memoryBlock_str) == TRUE)
        {
            verificationStatus_E = DAST_OK_E;
        }
        else
        {
            verificationStatus_E = DAST_CORRUPT_E;
        }
    }
    else
    {
        verificationStatus_E = DAST_UNKNOWN_E;
    }

    return verificationStatus_E;
}

static tB ramMirrorConsistent_B(tMemoryBlock_str memoryBlock_str)
{
    return (calcCrcOfRamMirror_U16(memoryBlock_str)
            == *(tU16*) memoryBlock_str.elements_str[memoryBlock_str.nofElements_U08
                    - 1].ramMirror_p);

}

static tU16 calcCrcOfRamMirror_U16(tMemoryBlock_str memoryBlock_str)
{
    tU16 crc_U16 = 0xFFFF;

    for (tU08 i_U08 = 0; i_U08 < (memoryBlock_str.nofElements_U08 - 1); i_U08++)
    {
        updateCrc(&crc_U16, memoryBlock_str.elements_str[i_U08].ramMirror_p,
                memoryBlock_str.elements_str[i_U08].bytes_U08);
    }

    return crc_U16;

}

static tB readMemoryBlockToRam(tMemoryBlock_str memoryBlock_str,
        tSector_E sector_E)
{
    //TODO: This shall return a boolean. What happens if no communication is possible with eeprom?
    tB ret_B = TRUE;
    tU16 sectorStartAddress_U16 = getSectorStartAddress_U16(sector_E);

    for (tU08 i_U08 = 0; i_U08 < memoryBlock_str.nofElements_U08; i_U08++)
    {
        Eepr_read(memoryBlock_str.elements_str[i_U08].ramMirror_p,
                sectorStartAddress_U16 + memoryBlock_str.address_U16
                        + memoryBlock_str.elements_str[i_U08].offset_U16,
                memoryBlock_str.elements_str[i_U08].bytes_U08);
    }

    return ret_B;
}

static tU16 getSectorStartAddress_U16(tSector_E sector_E)
{
    tU16 address_U16;
    address_U16 = (sector_E == PRIMARY_E) ?
    PRIMARY_MEM_ADDRESS :
                                            SECONDARY_MEM_ADDRESS;
    return address_U16;
}

//TODO: Settings block shall not be written zero to, it should be default values?
static tDast_verificationSolution_E clearMemoryBlock_E(
        tMemoryBlock_str memoryBlock_str)
{

    tDast_verificationSolution_E solution_E = DAST_LOST_E;
    tU16 crc_U16 = 0xFFFF;
    tB cleared_B = TRUE;
    tU08 buffer_aU08[BYTES_MAX] =
    { 0, 0, 0, 0 };

    //TODO: Borde skrivas om så att ram-spegeln nollas och därefter skriver man ner hela den!

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

    if (cleared_B != TRUE)
    {
        solution_E = DAST_UNFIXED_E;
    }

    return solution_E;
}

static void updateCrc(tU16 *crc_pU16, void *data_p, tU08 bytes_U08)
{
    while (bytes_U08--)
    {
        *crc_pU16 = _crc16_update(*crc_pU16, *(tU08*) data_p++);
    }
}

tDast_VerificationStatus_str Dast_getVerificationStatus_E(void)
{
    tDast_VerificationStatus_str verificationStatus_str =
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

tU16 Dast_getNofStarts_U16(void)
{
    return counterRamMirror_str.starts_U16;
}

void Dast_increaseNofStarts(void)
{
    tU16 crc_U16;

    INC_U16(counterRamMirror_str.starts_U16);
    crc_U16 = calcCrcOfRamMirror_U16(memoryBlock_pastr[COUNTERS_E]);
    *(tU16*) memoryBlock_pastr[COUNTERS_E].elements_str[memoryBlock_pastr[COUNTERS_E].nofElements_U08
            - 1].ramMirror_p = crc_U16;

    Eepr_write_B(
            memoryBlock_pastr[COUNTERS_E].elements_str[DAST_STARTS_E].ramMirror_p,
            PRIMARY_MEM_ADDRESS + memoryBlock_pastr[COUNTERS_E].address_U16
                    + memoryBlock_pastr[COUNTERS_E].elements_str[DAST_STARTS_E].offset_U16,
            memoryBlock_pastr[COUNTERS_E].elements_str[DAST_STARTS_E].bytes_U08);

    Eepr_write_B(
            memoryBlock_pastr[COUNTERS_E].elements_str[DAST_NOF_COUNTERS_E].ramMirror_p,
            PRIMARY_MEM_ADDRESS + memoryBlock_pastr[COUNTERS_E].address_U16
                    + memoryBlock_pastr[COUNTERS_E].elements_str[memoryBlock_pastr[COUNTERS_E].nofElements_U08
                            - 1].offset_U16,
            memoryBlock_pastr[COUNTERS_E].elements_str[memoryBlock_pastr[COUNTERS_E].nofElements_U08
                    - 1].bytes_U08);

    Eepr_write_B(
            memoryBlock_pastr[COUNTERS_E].elements_str[DAST_STARTS_E].ramMirror_p,
            SECONDARY_MEM_ADDRESS + memoryBlock_pastr[COUNTERS_E].address_U16
                    + memoryBlock_pastr[COUNTERS_E].elements_str[DAST_STARTS_E].offset_U16,
            memoryBlock_pastr[COUNTERS_E].elements_str[DAST_STARTS_E].bytes_U08);

    Eepr_write_B(
            memoryBlock_pastr[COUNTERS_E].elements_str[DAST_NOF_COUNTERS_E].ramMirror_p,
            SECONDARY_MEM_ADDRESS + memoryBlock_pastr[COUNTERS_E].address_U16
                    + memoryBlock_pastr[COUNTERS_E].elements_str[memoryBlock_pastr[COUNTERS_E].nofElements_U08
                            - 1].offset_U16,
            memoryBlock_pastr[COUNTERS_E].elements_str[memoryBlock_pastr[COUNTERS_E].nofElements_U08
                    - 1].bytes_U08);
}

void Dast_clearStat(void)
{
    clearRamMirror(memoryBlock_pastr[COUNTERS_E]);
    writeRamMirrorToSector_B(memoryBlock_pastr[COUNTERS_E], PRIMARY_E);
    writeRamMirrorToSector_B(memoryBlock_pastr[COUNTERS_E], SECONDARY_E);
    clearRamMirror(memoryBlock_pastr[TIMERS_E]);
    writeRamMirrorToSector_B(memoryBlock_pastr[TIMERS_E], PRIMARY_E);
    writeRamMirrorToSector_B(memoryBlock_pastr[TIMERS_E], SECONDARY_E);
}

static void clearRamMirror(tMemoryBlock_str memoryBlock_str)
{
    tU08 bytes_U08;
    tU08 * ramAddr_U08;

    for (tU08 i_U08 = 0; i_U08 < (memoryBlock_str.nofElements_U08 - 1); i_U08++)
    {
        bytes_U08 = memoryBlock_str.elements_str[i_U08].bytes_U08;
        ramAddr_U08 = (tU08*) memoryBlock_str.elements_str[i_U08].ramMirror_p;
        while (bytes_U08--)
        {
            *ramAddr_U08++ = 0;
        }
    }
    *(tU16*) memoryBlock_str.elements_str[memoryBlock_str.nofElements_U08 - 1].ramMirror_p =
            calcCrcOfRamMirror_U16(memoryBlock_str);
}
