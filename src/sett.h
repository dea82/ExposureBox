/*
 * sett.h
 *
 *  Created on: 18 mar 2015
 *      Author: andreas
 */

#ifndef SETT_H_
#define SETT_H_

#include "types.h"

typedef struct {
    tU08 length_U08;
    tU08 version_U08;
    tU16 pcbExpTime_U16;
    tU16 solderMaskExpTime_U16;
    tU16 solderMaskHardeningTime_U16;
    tU16 customExpTime_U16;
    tB   abscenceWarning_B;
    tU16 crc_U16;
} tSett_SettingsSection_str;

#endif /* SETT_H_ */
