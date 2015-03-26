/*
 * spii.h
 *
 *  Created on: 10 maj 2014
 *      Author: andreas
 */

#ifndef SPII_H_
#define SPII_H_

#include "types.h"

typedef enum {SPII_CS_LOW = 0, SPII_CS_HIGH} csState_t;

void Spii_init(void);
void Spii_write(tU08 data_U08);
tU08 Spii_getByte_U08(void);

#endif /* SPII_H_ */
