/*
 * ledc.h
 *
 *  Created on: 15 maj 2014
 *      Author: andreas
 */

#ifndef LEDC_H_
#define LEDC_H_

#include "types.h"

void Ledc_init(void);
void Ledc_10ms(void);
void Ledc_setStatus(tB status_B);
void Ledc_toggleStatus(void);

#endif /* LEDC_H_ */
