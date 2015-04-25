/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : main.c
 * Date    : 2 maj 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "conf.h"
#include "seri.h"
#include "spii.h"
#include "eepr.h"
#include "lcdc.h"
#include "types.h"
#include "ledc.h"
#include "butt.h"
#include "lise.h"
#include "pomm.h"
#include "sche.h"
#include "colo.h"
#include "buzz.h"
#include "dast.h"

#include <util/delay.h>


int main(void)
{
    Seri_init();
    Spii_init();
    //TODO: Replace with nvmm?
    Eepr_init();
    Pomm_init();
    Lcdc_init();
    Lise_init();
    Ledc_init();
    Butt_init();
    Colo_init();
    Sche_init();
    Buzz_init();
    Dast_init();

    Sche_addTask(0, Butt_loop,  BUTT_T);
    Sche_addTask(1, Ledc_10ms, 10);
    Sche_addTask(2, Colo_loop, COLO_T);
    Sche_addTask(3, Buzz_10ms, 10);
    Sche_addTask(4, Lcdc_10ms, LCDC_T);


    Sche_startScheduler();
    for(;;)
    {
        /* Dispatch the tasks */
        Sche_dispatchTasks();
        //Seri_writeByte(0x55);

    }

}
