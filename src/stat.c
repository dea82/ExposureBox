/*
 * stat.c
 *
 *  Created on: 17 mar 2015
 *      Author: andreas
 *
 *      This module handles the statistics for the Exposure Box.
 *      It records the following:
 *          - Number of starts, 0-65536 [-]
 *          - Online time, 0-4294967296 [s]
 *          - Online with lights on time, 0-4294967296 [s]
 *          -
 */

#include "stat.h"

#include "dast.h"
#include "types.h"

static tB incCounter_B(tStat_Counters_E counter_E);

tB Stat_incStarts(void)
{
    return incCounter_B(STAT_STARTS_E);
}

void Stat_init(void)
{

}

void Stat_loop(void)
{
    //TODO: Updater timers.
}

static tB incCounter_B(tStat_Counters_E counter_E)
{

    return FALSE;
}

void Stat_clearStat(void)
{

}
