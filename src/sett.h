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
    tU16 pcbExpTime_U16;
    tU16 solderMaskExpTime_U16;
    tU16 solderMaskHardeningTime_U16;
    tU16 customExpTime_U16;
    tU16 lightOnCalibration_U16;
    tU16 lightOffCalibration_U16;
    tU08 generalSettings1_U08;
} tSett_Settings_str;

#endif /* SETT_H_ */
