/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : lise.c
 * Date    : 18 maj 2014
 * 
 * Summary : Light sensor module
 *
 * This module measures light.
 *
 *****************************************************************************/

/******************************************************************************
 * Header Files
 *****************************************************************************/
#include <avr/io.h>
#include "conf.h"
#include "gpio.h"
#include "types.h"

/******************************************************************************
 * Function Protoypes
 *****************************************************************************/

/******************************************************************************
 * Functions
 *****************************************************************************/

void Lise_init(void)
{
    gpio_conf(LIGHT_SENSOR_CFG, INPUT, NOPULLUP);

    /*
     * Set ADC prescaler, should be 50 - 200 kHz.
     * Prescaler 128 => 125kHz @ 16Mhz
     */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    /* Set ADC reference to AVCC */
    ADMUX |= (1 << REFS0);

    /* Left adjust ADC result */
    ADMUX |= (1 << ADLAR);

    /* Connect ADC6 to ADC */
    ADMUX |= (1 << MUX2) | (1 << MUX1);

    /* Set ADC to Free-Running Mode */
    ADCSRA |= (1 << ADATE);

    /* Enable ADC */
    ADCSRA |= (1 << ADEN);

    /* Start ADC conversion */
    ADCSRA |= (1 << ADSC);
}

void Lise_getLightState(void)
{

}

tU08 Lise_getRawValue(void)
{
    return (ADCH);
}
