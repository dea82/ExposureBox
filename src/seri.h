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
void Seri_writeByte(const char data);
void Seri_writeString(const char *string);

#endif /* SERI_H_ */
