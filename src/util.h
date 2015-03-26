/*
 * util.h
 *
 *  Created on: 5 nov 2013
 *      Author: andreas
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <avr/pgmspace.h>

/* Type safe and safe for double evaluation */
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

typedef uint8_t clock_t[4];

void Util_safeUCharIncrement(unsigned char *c);
void Util_safeUIntIncrement(unsigned int *c);
void Util_safeULongIncrement(unsigned long *c);

void Util_concatString(char *original, char *add);
char* Util_digitToChar(const uint8_t digit);


#endif /* UTIL_H_ */
