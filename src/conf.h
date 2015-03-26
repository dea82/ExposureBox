/*
 * conf.h
 *
 *  Created on: 10 maj 2014
 *      Author: andreas
 */

#ifndef CONF_H_
#define CONF_H_

#include <avr/io.h>
#include "types.h"
#include "stat.h"
#include "sett.h"

/* Serial communication */
#define BAUD_RATE 9600

/*************************************************
 * EEPROM STRUCTURE
 *************************************************/
/* All data in the EEPROM is mirrored. There are three sections:
 * - Shutdown section, which can be written to at shutdown. Time critical, has it's own CRC.
 * - Usage data.
 * - Settings
 */

/* ATTENTION: If the EEPROM structure is changed in any way increment this counter!!! */
#define SW_VERSION "DEV"

//#define SHUTDOWN_SECTION_ADDR 0x0
//#define STAT_SECTION_ADDR (SHUTDOWN_SECTION_ADDR + ((sizeof(tStat_ShutdownSection_str) + CRC_LENGTH) * 2))
//#define SETT_SECTION_ADDR (STAT_SECTION_ADDR + ((sizeof(tSett_EepromStatisticsSection_str) + CRC_LENGTH) * 2))


/*************************************************
 * PIN CONFIGURATION
 *************************************************/
#define LED_CFG                 D, 4, 4
#define MISO_CFG                B, 6, 6
#define MOSI_CFG                B, 5, 5
#define SCK_CFG                 B, 7, 7
#define EEPROM_CS_CFG           B, 4, 4
#define LCD_BACKLIGHT_CFG       A, 7, 7
#define LCD_RS_CFG              C, 1, 1
#define LCD_RW_CFG              C, 2, 2
#define LCD_ENABLE_CFG          C, 3, 3
#define LCD_D_CFG               C, 7, 4
#define LCD_D4_CFG              C, 4, 4
#define LCD_D5_CFG              C, 5, 5
#define LCD_D6_CFG              C, 6, 6
#define LCD_D7_CFG              C, 7, 7
#define LEFT_BUTTON_CFG         D, 6, 6
#define RIGHT_BUTTON_CFG        D, 5, 5
#define LIGHT_SENSOR_CFG        A, 6, 6
#define RELAY_1_CFG             D, 2, 2
#define RELAY_2_CFG             D, 3, 3
#define PWR_LOSS_DETECTOR_CFG   B, 3, 3
#define BUZZ_CFG                D, 7, 7

#endif /* CONF_H_ */
