/*
 * dast.c
 *
 *  Created on: 14 okt 2015
 *      Author: andreas
 */

#include "dast.h"
#include "sett.h"
#include "stat.h"
#include "avr/pgmspace.h"
#include "types.h"
#include "eepr.h"
#include "conf.h"
#include "util/crc16.h"

#define CRC_BYTES 2

typedef enum
{
    PRIMARY_E, SECONDARY_E
} tSector_E;

typedef enum
{
    UNKNOWN_E, HEALTHY_E, CORRUPT_E, COMM_ERROR_E,
} tMemBlockSectorHealth_E;

typedef const struct
{
    tU16 startAddressInSector_U16;
    tU08 size_U08;
} tMemBlockAddressing_str;

static tMemBlockSectorHealth_E readMemBlockFromSector_E(
        const tDast_MemBlock_E memBlock_E, tU08 *dest_U08,
        const tSector_E sector_E);

static tU16 getMemBlockStartAddress_U16(tDast_MemBlock_E memBlock_E,
        tSector_E sector_E);

static tU08 getMemBlockSize_U08(tDast_MemBlock_E memBlock_E);
static tU16 calcCrc_U16(tU08 *data_paU08, tU08 size_U08);
static tB readCrc_B(tDast_MemBlock_E memBlock_E, tU16 *crc_U16,
        tSector_E sector_E);
static tB updateMemoryBlockInSector_B(tDast_MemBlock_E memBlock_E,
        tU08 *data_paU08, tSector_E sector_E);
static tB writeCrc(tDast_MemBlock_E memBlock_E, tU08 *data_paU08,
        tSector_E sector_E);
PROGMEM tMemBlockAddressing_str memBlockAddressing_astr[DAST_NOF_MEMORY_BLOCKS_E] =
{
{ 0, sizeof(tStat_Counters_str) },
{ sizeof(tStat_Counters_str) + CRC_BYTES, sizeof(tStat_Timers_str) },
{ sizeof(tStat_Counters_str) + CRC_BYTES + sizeof(tStat_Timers_str) + CRC_BYTES,
        sizeof(tSett_Settings_str) } };

PROGMEM const tU16 sectorStartAddress_U16[2] =
{ PRI_SECTOR_START_ADDR, SEC_SECTOR_START_ADDR };

tMemBlockSectorHealth_E memBlockHealth_aE[DAST_NOF_MEMORY_BLOCKS_E][2];

tDast_ReadMemBlockStatus_E Dast_ReadMemBlock_E(
        const tDast_MemBlock_E memBlock_E, tU08 *dest_paU08)
{
    tDast_ReadMemBlockStatus_E status_E;

    if (readMemBlockFromSector_E(memBlock_E, dest_paU08, PRIMARY_E)
            == HEALTHY_E)
    {
        //TODO: What to do with the return value?
        updateMemoryBlockInSector_B(memBlock_E, dest_paU08, SECONDARY_E);
        status_E = DAST_NO_DATA_LOSS_E;
    }
    else if (readMemBlockFromSector_E(memBlock_E, dest_paU08, SECONDARY_E)
            == HEALTHY_E)
    {
        updateMemoryBlockInSector_B(memBlock_E, dest_paU08, PRIMARY_E);
        status_E = DAST_PARTIAL_DATA_LOSS_E;
    }
    else
    {
        status_E = DAST_TOTAL_DATA_LOSS_E;
    }

    return status_E;
}

static tMemBlockSectorHealth_E readMemBlockFromSector_E(
        const tDast_MemBlock_E memBlock_E, tU08 *dest_paU08,
        const tSector_E sector_E)
{
    tMemBlockSectorHealth_E status_E;
    tU16 crc_U16;

    if ((Eepr_read_B(dest_paU08,
            getMemBlockStartAddress_U16(memBlock_E, sector_E),
            getMemBlockSize_U08(memBlock_E)) == TRUE)
            && (readCrc_B(memBlock_E, &crc_U16, sector_E) == TRUE))
    {

        if (calcCrc_U16(dest_paU08, getMemBlockSize_U08(memBlock_E)) == crc_U16)
        {
            status_E = HEALTHY_E;
        }
        else
        {
            status_E = CORRUPT_E;
        }
    }
    else
    {
        status_E = COMM_ERROR_E;
    }

    return status_E;
}

static tU16 getMemBlockStartAddress_U16(tDast_MemBlock_E memBlock_E,
        tSector_E sector_E)
{
    return (tU16) (memBlockAddressing_astr[memBlock_E].startAddressInSector_U16
            + sectorStartAddress_U16[sector_E]);
}

static tU08 getMemBlockSize_U08(tDast_MemBlock_E memBlock_E)
{
    return memBlockAddressing_astr[memBlock_E].size_U08;
}

static tU16 getMemBlockCrcAddress_U16(tDast_MemBlock_E memBlock_E,
        tSector_E sector_E)
{
    return (getMemBlockStartAddress_U16(memBlock_E, sector_E)
            + getMemBlockSize_U08(memBlock_E));
}

static tU16 calcCrc_U16(tU08 *data_paU08, tU08 size_U08)
{
    tU16 crc_U16 = 0xFFFF;
    while (size_U08--)
    {
        crc_U16 = _crc16_update(crc_U16, *data_paU08++);
    }
    return crc_U16;
}

static tB readCrc_B(tDast_MemBlock_E memBlock_E, tU16 *crc_U16,
        tSector_E sector_E)
{
    return (Eepr_read_B((tU08*) crc_U16,
            getMemBlockCrcAddress_U16(memBlock_E, sector_E), CRC_BYTES));

}

static tB updateMemoryBlockInSector_B(tDast_MemBlock_E memBlock_E,
        tU08 *data_paU08, tSector_E sector_E)
{
    tB ret_B;
    if (Eepr_update_B(data_paU08,
            getMemBlockStartAddress_U16(memBlock_E, sector_E),
            getMemBlockSize_U08(memBlock_E)) == TRUE)
    {
        if (writeCrc(memBlock_E, data_paU08, sector_E) == TRUE)
        {
            ret_B = TRUE;
        }
        else
        {
            ret_B = FALSE;
        }
    }
    else
    {
        ret_B = FALSE;
    }
    return ret_B;
}

static tB writeCrc(tDast_MemBlock_E memBlock_E, tU08 *data_paU08,
        tSector_E sector_E)
{
    return FALSE;
}
