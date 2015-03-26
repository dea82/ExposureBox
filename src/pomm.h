/*
 * pomm.h
 *
 *  Created on: 29 maj 2014
 *      Author: andreas
 */

#ifndef POMM_H_
#define POMM_H_

typedef enum
{
    POMM_EEPROM_STATUS_OK_E,
    POMM_EEPROM_STATUS_CORRUPT_E,
    POMM_EEPROM_STATUS_PENDING_E
} tPOMM_VERIFICATION_STATUS_E;

void Pomm_init(void);
tPOMM_VERIFICATION_STATUS_E Pomm_getVerificationState_E(void);
tB Pomm_powerDown_B(void);

#endif /* POMM_H_ */
