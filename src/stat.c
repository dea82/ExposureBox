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

void Stat_init(void)
{

}

void Stat_loop(void)
{
    //TODO: Updater timers.
}

tB Stat_incCounter_B(tStat_Counter_E counter_E)
{
    return Dast_incCounter_B(counter_E);
}

tB Stat_incTimer_B(tStat_Timer_E timer_E)
{
    return Dast_incTimer_B(timer_E);
}
