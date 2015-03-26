/*
 * stat.h
 *
 *  Created on: 18 mar 2015
 *      Author: andreas
 */

#ifndef STAT_H_
#define STAT_H_


typedef enum
{
    STAT_ONLINE_E,
    STAT_LIGHTS_ON_E
} tStat_Timer_E;

typedef enum
{
    STAT_STARTS_E,
    STAT_LIGHTS_ACTIVATION_E,
    STAT_USER_ABSENCE_WARNINGS_E,
    STAT_PCB_EXP_E,
    STAT_SOLDER_MASK_EXP_E,
    STAT_SOLDER_MASK_HARDENING_E,
    STAT_CUSTOM_EXP_TIME_E
} tStat_Counter_E;

#endif /* STAT_H_ */
