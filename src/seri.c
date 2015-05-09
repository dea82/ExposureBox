/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : seri.c
 * Date    : 10 maj 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

#include "conf.h"
#include <avr/io.h>

#define BRC ((F_CPU/16/BAUD_RATE) - 1)

void Seri_init(void)
{
  /* Write baud rate high/low byte. */
  UBRRH = (BRC>>8);
  UBRRL = BRC;
  /* Enable RX and TX */
  UCSRB = (1<<RXEN)|(1<<TXEN);
  /* UART 8-bit */
  UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}

void Seri_writeByte(const char data)
{
    while (( UCSRA & (1 << UDRE )) == 0) {}; /* Do nothing until UDR is ready for more data tobe written to it */
    UDR = data;
}

void Seri_writeString(const char *string)
{
    while(*string)
    {
        Seri_writeByte(*string++);
    }
}
