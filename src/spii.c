/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : spii.c
 * Date    : 10 maj 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/
#include <avr/io.h>
#include "gpio.h"
#include "conf.h"
#include "eepr.h"

void Spii_init(void)
{
    /* MISO */
    gpio_conf(MISO_CFG, INPUT, NOPULLUP);
    /* MOSI */
    gpio_conf(MOSI_CFG, OUTPUT, 0);
    /* SCK */
    gpio_conf(SCK_CFG, OUTPUT, 0);

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    SPSR |= (1 << SPI2X);
}

void Spii_write(tU08 data_U08)
{
    SPDR = data_U08;
    while (!(SPSR & (1 << SPIF)))
        ;
}

tU08 Spii_getByte_U08(void)
{
    Spii_write(DUMMY);
    return SPDR;
}
