
#ifndef MSTMREQUIRED_H_
#define MSTMREQUIRED_H_

#include "sc_types.h"
#include "Mstm.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file This header defines prototypes for all functions that are required by the state machine implementation.

This is a state machine uses time events which require access to a timing service. Thus the function prototypes:
	- mstm_setTimer and
	- mstm_unsetTimer
are defined.

This state machine makes use of operations declared in the state machines interface or internal scopes. Thus the function prototypes:
	- mstmIface_nofExpTypes
	- mstmIface_printStartupCheck
	- mstmIface_printStartUpCheckFailed
	- mstmIface_printStartUpCheckOk
	- mstmIface_printStartScreen
	- mstmIface_printSelectExpType
	- mstmIface_printExposure
	- mstmIface_printExposing
	- mstmIface_printExposurePaused
	- mstmIface_printExposureDone
	- mstmIface_printSetTimersDefaultValue
	- mstmIface_printStatistics
	- mstmIface_printNofStarts
	- mstmIface_printClearStatQuestion
	- mstmIface_printConfirmClear
	- mstmIface_printClearing
	- mstmIface_printClearDone
	- mstmIface_writeCustomTimer
	- mstmIface_playToneWarning
	- mstmIface_updateStartsStat
	- mstmIface_clearStat
are defined.

These functions will be called during a 'run to completion step' (runCycle) of the statechart. 
There are some constraints that have to be considered for the implementation of these functions:
	- never call the statechart API functions from within these functions.
	- make sure that the execution time is as short as possible.
 
*/
extern sc_integer mstmIface_nofExpTypes();
extern void mstmIface_printStartupCheck();
extern void mstmIface_printStartUpCheckFailed();
extern void mstmIface_printStartUpCheckOk();
extern void mstmIface_printStartScreen();
extern void mstmIface_printSelectExpType(const sc_integer type);
extern void mstmIface_printExposure(const sc_integer type);
extern void mstmIface_printExposing();
extern void mstmIface_printExposurePaused();
extern void mstmIface_printExposureDone();
extern void mstmIface_printSetTimersDefaultValue();
extern void mstmIface_printStatistics();
extern void mstmIface_printNofStarts();
extern void mstmIface_printClearStatQuestion();
extern void mstmIface_printConfirmClear();
extern void mstmIface_printClearing();
extern void mstmIface_printClearDone();
extern void mstmIface_writeCustomTimer();
extern void mstmIface_playToneWarning();
extern void mstmIface_updateStartsStat();
extern void mstmIface_clearStat();


//
// This is a timed state machine that requires timer services
// 

//! This function has to set up timers for the time events that are required by the state machine.
/*! 
	This function will be called for each time event that is relevant for a state when a state will be entered.
	\param evid An unique identifier of the event.
	\time_ms The time in milli seconds
	\periodic Indicates the the time event must be raised periodically until the timer is unset 
*/
extern void mstm_setTimer(Mstm* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic);

//! This function has to unset timers for the time events that are required by the state machine.
/*! 
	This function will be called for each time event taht is relevant for a state when a state will be left.
	\param evid An unique identifier of the event.
*/
extern void mstm_unsetTimer(Mstm* handle, const sc_eventid evid);

#ifdef __cplusplus
}
#endif 

#endif /* MSTMREQUIRED_H_ */
