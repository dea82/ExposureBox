/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : ledc.c
 * Date    : 15 maj 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

#include "gpio.h"
#include "conf.h"
#include "types.h"
#include "ledc.h"

void Ledc_init(void)
{
    gpio_conf(LED_CFG, OUTPUT, 0);
}

void Ledc_10ms(void)
{
}

void Ledc_setStatus(tB status_B)
{
    status_B ? gpio_set(LED_CFG): gpio_clr(LED_CFG);
}

void Ledc_toggleStatus(void)
{
    //TODO: Does not work!!!
    gpio_toggle(LED_CFG);
}
