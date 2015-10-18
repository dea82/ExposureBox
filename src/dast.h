/*
 * dast.h
 *
 *  Created on: 14 okt 2015
 *      Author: andreas
 */

#ifndef DAST_H_
#define DAST_H_

typedef enum
{
    DAST_COUNTERS_E,
    DAST_TIMERS_E,
    DAST_SETTINGS_E,
    DAST_NOF_MEMORY_BLOCKS_E
}tDast_MemBlock_E;

typedef enum
{
    DAST_COMPLETE_DATA_WRITTEN_OK_E,
    DAST_PARTIAL_DATA_WRITTEN_E,
    DAST_NO_DATA_WRITTEN_E
} tDast_WriteMemBlockStatus_E;

typedef enum
{
    DAST_NO_DATA_LOSS_E,
    DAST_PARTIAL_DATA_LOSS_E,
    DAST_TOTAL_DATA_LOSS_E
} tDast_ReadMemBlockStatus_E;

#endif /* DAST_H_ */
