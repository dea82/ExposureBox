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

#include "dast.h"
#include "stat.h"
#include "types.h"
#include "dast.h"

static tB incCounter_B(tDast_Counters_E counter_E);

tB Stat_incStarts(void)
{
    return incCounter_B(DAST_STARTS_E);
}

void Stat_init(void)
{

}

void Stat_loop(void)
{
    //TODO: Updater timers.
}

static tB incCounter_B(tDast_Counters_E counter_E)
{
    return FALSE;
}
