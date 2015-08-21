/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : sche.c
 * Date    : 29 maj 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

/******************************************************************************
 * Header files
 *****************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "sche.h"
#include "seri.h"
#include "types.h"
#include "stmt.h"
#include "ledc.h"
/******************************************************************************
 * Defines
 *****************************************************************************/
#define MAX_TASKS 6

#define RUNNABLE (0x00)
#define RUNNING  (0x01)
#define STOPPED  (0x02)
#define ERROR    (0x03)
/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef struct __tcb_t
{
    uint8_t id; // task ID
    task_t task; // pointer to the task
    // delay before execution
    uint16_t delay, period;
    uint8_t status; // status of task
} tcb_t;

/******************************************************************************
 * Global variables
 *****************************************************************************/
tcb_t task_list[MAX_TASKS];
tU08 c_U08 = 0;
/******************************************************************************
 * Function prototypes
 *****************************************************************************/
static void initScheduler(void);
/******************************************************************************
 * Functions
 *****************************************************************************/
void Sche_init(void)
{
    /* Timer0 Operation with prescaler 8 */
    TCCR0 = (1 << CS01);

    /* Init counter */
    TCNT0 = 0x38;

    /* Enable Timer0 overflow interrupt */
    TIMSK = (1 << TOIE0);

    initScheduler();
    initTimers();
}

/* Called every 100us @ 16Mhz */
ISR(TIMER0_OVF_vect)
{
    //TODO: Examine assembly code and compensate for that, remember a couple of clock cycles to reach ISR.
    TCNT0 = 0x38;

    /* Tick generated every 1ms*/
    if(c_U08++ >= 10)
    {
        checkTimers(1);
        c_U08 = 0;
        tU08 i_U08;
        for(i_U08 = 0; i_U08 < MAX_TASKS; i_U08 ++)
        {
            if (task_list[i_U08].delay > 0)
            {
                task_list[i_U08].delay--;
            }
            if ( (task_list[i_U08].status == RUNNING) && !task_list[i_U08].delay)
            {
                task_list[i_U08].status = ERROR;
            }
        }
    }
}


static void initScheduler(void)
{
    uint8_t i;
    for( i = 0; i<MAX_TASKS; i++)
    {
        task_list[i].id = 0;
        task_list[i].task = (task_t)0x00;
        task_list[i].delay = 0;
        task_list[i].period = 0;
        task_list[i].status = STOPPED;
    }
}

void Sche_addTask(uint8_t id, task_t task, uint16_t period)
{
    uint8_t idx = 0, done = 0x00;
    while( idx < MAX_TASKS )
    {
        if( task_list[idx].status == STOPPED )
        {
            task_list[idx].id = id;
            task_list[idx].task = task;
            task_list[idx].delay = period;
            task_list[idx].period = period;
            task_list[idx].status = RUNNABLE;
            done = 0x01;
        }
        if( done ) break;
        idx++;
    }

}

void Sche_startScheduler(void)
{
    sei();
}

void Sche_dispatchTasks(void)
{
    static tU08 cMax = 0;
    uint8_t i;
    for( i = 0; i < MAX_TASKS; i++)
    {
        // check for a valid task ready to run
        if( !task_list[i].delay &&
             task_list[i].status == RUNNABLE )
        {
            if(cMax < c_U08)
            {
                cMax = c_U08;
                //Seri_writeByte(cMax);
            }
            // task is now running
            task_list[i].status = RUNNING;
            task_list[i].delay =
                            task_list[i].period;
            // call the task
            (*task_list[i].task)();

            // task is runnable again
            task_list[i].status = RUNNABLE;
        }
    }
}
