/*
 * util.c
 *
 *  Created on: 5 nov 2013
 *      Author: andreas
 */
#include <limits.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "util.h"

void Util_concatString(char *original, char *add)
{
   while(*original)
      original++;

   while(*add)
   {
      *original = *add;
      add++;
      original++;
   }
   *original = '\0';
}

char* Util_digitToChar(const uint8_t digit)
{
	char ascii = 35; /* # */
	char * str;

	if(digit >= 0 && digit < 10)
	{
		ascii = digit + '0';
	}

	str = &ascii;

	return str;
}

char* Util_clockToString(const clock_t clock)
{
	char * str_p;
	char str[5];

	str_p = &str[0];

	return str_p;
}
