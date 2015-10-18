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
#include <util/delay.h>
#include "dast.h"


int main(void)
{
    /* Check for WDT reset (save code size and assume true) - if a runaway pointer enables it,
     * then it must be disabled here because it's kept after a reset! Ref. AVR132 chap 2.4. */
    WDTCR = 0;

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
