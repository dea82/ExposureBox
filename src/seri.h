/*
 * seri.h
 *
 *  Created on: 10 maj 2014
 *      Author: andreas
 */

#ifndef SERI_H_
#define SERI_H_

#include <avr/io.h>

void Seri_init(void);
void Seri_writeByte(const uint8_t data);
void Seri_writeString(const uint8_t *string);

#endif /* SERI_H_ */
