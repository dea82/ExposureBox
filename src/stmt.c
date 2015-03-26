/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : stmt.c
 * Date    : 25 okt 2014
 * 
 * Summary : TODO
 *
 *****************************************************************************/

/******************************************************************************
 * Header files
 *****************************************************************************/
#include "src-gen/mstm.h"
#include "src-gen/sc_types.h"
#include "stmt.h"

#define MAX_NOF_TIMERS 2

struct timer {
        // data from statemachines
        void* statemachineHandle;
        sc_eventid evid;
        sc_integer time_ms;
        sc_boolean periodic;
        // pointer to callback function for raising the event
        raiseEventCallback_t callback_fun;

        // internal values
        // current timeout time for the event
        sc_integer timeout_ms;
        // 1 if this timer is currently in use
        uint8_t in_use;
};

static struct timer timers[MAX_NOF_TIMERS];
/******************************************************************************
 * Function prototypes
 *****************************************************************************/
/******************************************************************************
 * Functions
 *****************************************************************************/

// must be called before all other timer functions
void initTimers(void) {
        uint8_t i;
        for (i = 0; i < MAX_NOF_TIMERS; ++i) {
                timers[i].in_use = 0;
        }
}

// calculate new timeout for a timer
static void setTimeout(struct timer *tim) {
        tim->timeout_ms = tim->time_ms;
        tim->in_use = 1;
}

void Stmt_setTimer(Mstm* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic, raiseEventCallback_t callback)
{
    uint8_t i;
    for (i = 0; i < MAX_NOF_TIMERS; ++i) {
            if (timers[i].in_use)
                    continue;

            timers[i].statemachineHandle = handle;
            timers[i].evid = evid;
            timers[i].time_ms = time_ms;
            timers[i].periodic = periodic;
            timers[i].callback_fun = callback;
            setTimeout(&timers[i]);
            return;
    }
    //error: no free timer
}

void Stmt_unsetTimer(const sc_eventid evid)
{
    int8_t i;
    for (i = 0; i < MAX_NOF_TIMERS; ++i) {
            if (timers[i].evid != evid || !(timers[i].in_use))
                    continue;
            timers[i].in_use = 0;
    }
}
// helper: called if the timer timed out
static void dispatchTimerEvent(struct timer *tim) {
        if (tim->callback_fun)
                tim->callback_fun(tim->statemachineHandle, tim->evid);

        if (tim->periodic)
                setTimeout(tim);
        else
                tim->in_use = 0;
}

/**
 * Checks if a timer has a timeout.
 *
 * \param deltaT_ms Time in milliseconds since the last call.
 */
void checkTimers(sc_integer deltaT_ms) {
        uint8_t i;

        if(deltaT_ms <= 0)
                return;

        for (i = 0; i < MAX_NOF_TIMERS; ++i) {
                if (!timers[i].in_use)
                        continue;

                timers[i].timeout_ms -= deltaT_ms;
                if (timers[i].timeout_ms > 0)
                        continue;

                dispatchTimerEvent(&timers[i]);
        }
}
