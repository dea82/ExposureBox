
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "Mstm.h"
#include "MstmRequired.h"
/*! \file Implementation of the state machine 'mstm'
*/

// prototypes of all internal functions

static void mstm_enact_SequenceImpl(Mstm* handle);
static void mstm_exact_SequenceImpl(Mstm* handle);
static void mstm_dhenseq_SequenceImpl(Mstm* handle);
static void mstm_dhenseq_main_NormalFunction_NormalFunction_CustomExposureTime_SequenceImpl(Mstm* handle);
static void mstm_shenseq_SequenceImpl(Mstm* handle);
static void mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(Mstm* handle);
static void mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_Settings_Clear_SequenceImpl(Mstm* handle);
static void mstm_react_UserAbsence_UserPresent_UserPresent_TimeoutCountDown(Mstm* handle);
static void mstm_react_UserAbsence_UserPresent_UserPresent_UserInput(Mstm* handle);
static void mstm_react_UserAbsence_UserAbsence(Mstm* handle);
static void mstm_react_main_VerifyEEprom(Mstm* handle);
static void mstm_react_main_EEpromVerifiedOk(Mstm* handle);
static void mstm_react_main_EEpromVerifiedCorrupt(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_StartScreen(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_SelectPCB(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Exposure(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Exposing(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_ExposureDone(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_ExposurePaused(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Statistics(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing(Mstm* handle);
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone(Mstm* handle);
static void mstm_react_main_UserAbsence(Mstm* handle);
static void mstm_react_main_PowerDown_PowerDown_PowerDownState(Mstm* handle);
static void mstm_react_main_PowerDown_PowerDown_Recover(Mstm* handle);
static void mstm_clearInEvents(Mstm* handle);
static void mstm_clearOutEvents(Mstm* handle);


void mstm_init(Mstm* handle)
{
	int i;

	for (i = 0; i < MSTM_MAX_ORTHOGONAL_STATES; ++i)
		handle->stateConfVector[i] = Mstm_last_state;
	
	for (i = 0; i < MSTM_MAX_HISTORY_STATES; ++i)
		handle->historyVector[i] = Mstm_last_state;
	
	handle->stateConfVectorPosition = 0;

	mstm_clearInEvents(handle);
	mstm_clearOutEvents(handle);

	/* Default init sequence for statechart mstm */
	handle->iface.expType = 0;
	handle->iface.digit = 0;
	handle->iface.userAbsence = bool_false;

}

void mstm_enter(Mstm* handle)
{
	/* Default enter sequence for statechart mstm */
	mstm_enact_SequenceImpl(handle);
	/* 'default' enter sequence for region UserAbsence */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state UserPresent */
	/* Entry action for state 'UserPresent'. */
	handle->iface.userAbsence = bool_false;
	/* 'default' enter sequence for region UserPresent */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state TimeoutCountDown */
	/* Entry action for state 'TimeoutCountDown'. */
	mstm_setTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown_tev0_raised) , 30 * 1000, bool_false);
	handle->stateConfVector[0] = Mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown;
	handle->stateConfVectorPosition = 0;
	/* 'default' enter sequence for region main */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state VerifyEEprom */
	/* Entry action for state 'VerifyEEprom'. */
	mstmIface_printStartupCheck();
	handle->stateConfVector[1] = Mstm_main_VerifyEEprom;
	handle->stateConfVectorPosition = 1;
}

void mstm_exit(Mstm* handle)
{
	/* Default exit sequence for statechart mstm */
	/* Default exit sequence for region UserAbsence */
	/* Handle exit of all possible states (of UserAbsence) at position 0... */
	switch(handle->stateConfVector[ 0 ]) {
		case Mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown : {
			/* Default exit sequence for state TimeoutCountDown */
			handle->stateConfVector[0] = Mstm_last_state;
			handle->stateConfVectorPosition = 0;
			/* Exit action for state 'TimeoutCountDown'. */
			mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown_tev0_raised) );		
			break;
		}
		case Mstm_UserAbsence_UserPresent_UserPresent_UserInput : {
			/* Default exit sequence for state UserInput */
			handle->stateConfVector[0] = Mstm_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		case Mstm_UserAbsence_UserAbsence : {
			/* Default exit sequence for state UserAbsence */
			handle->stateConfVector[0] = Mstm_last_state;
			handle->stateConfVectorPosition = 0;
			break;
		}
		default: break;
	}
	/* Default exit sequence for region main */
	/* Handle exit of all possible states (of main) at position 1... */
	switch(handle->stateConfVector[ 1 ]) {
		case Mstm_main_VerifyEEprom : {
			/* Default exit sequence for state VerifyEEprom */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_EEpromVerifiedOk : {
			/* Default exit sequence for state EEpromVerifiedOk */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'EEpromVerifiedOk'. */
			mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_EEpromVerifiedOk_tev0_raised) );		
			break;
		}
		case Mstm_main_EEpromVerifiedCorrupt : {
			/* Default exit sequence for state EEpromVerifiedCorrupt */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
			/* Default exit sequence for state StartScreen */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
			/* Default exit sequence for state SelectPCB */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Exposure : {
			/* Default exit sequence for state Exposure */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Exposing : {
			/* Default exit sequence for state Exposing */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
			/* Default exit sequence for state Digit1 */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
			/* Default exit sequence for state Digit2 */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
			/* Default exit sequence for state Digit3 */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
			/* Default exit sequence for state Digit4 */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'Digit4'. */
			mstmIface_writeCustomTimer();
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
			/* Default exit sequence for state ExposureDone */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
			/* Default exit sequence for state ExposurePaused */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
			/* Default exit sequence for state TimersDefault */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
			/* Default exit sequence for state Statistics */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
			/* Default exit sequence for state NofStarts */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
			/* Default exit sequence for state ClearQuestion */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
			/* Default exit sequence for state ConfirmClear */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
			/* Default exit sequence for state ClearPending */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'ClearPending'. */
			mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
			/* Default exit sequence for state Clearing */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'Clearing'. */
			mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
			/* Default exit sequence for state ClearDone */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_UserAbsence : {
			/* Default exit sequence for state UserAbsence */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_PowerDown_PowerDown_PowerDownState : {
			/* Default exit sequence for state PowerDownState */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			break;
		}
		case Mstm_main_PowerDown_PowerDown_Recover : {
			/* Default exit sequence for state Recover */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'Recover'. */
			mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_PowerDown_PowerDown_Recover_tev0_raised) );		
			break;
		}
		default: break;
	}
	mstm_exact_SequenceImpl(handle);
}

static void mstm_clearInEvents(Mstm* handle) {
	handle->iface.leftButtonPressed_raised = bool_false;
	handle->iface.leftButtonReleased_raised = bool_false;
	handle->iface.rightButtonPressed_raised = bool_false;
	handle->iface.eepromDataVerifiedOk_raised = bool_false;
	handle->iface.eepromDataVerifiedCorrupt_raised = bool_false;
	handle->iface.powerDown_raised = bool_false;
	handle->iface.timerTimeout_raised = bool_false;
	handle->iface.timerRunning_raised = bool_false;
	handle->timeEvents.mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown_tev0_raised = bool_false; 
	handle->timeEvents.mstm_main_EEpromVerifiedOk_tev0_raised = bool_false; 
	handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised = bool_false; 
	handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised = bool_false; 
	handle->timeEvents.mstm_main_PowerDown_PowerDown_Recover_tev0_raised = bool_false; 
}

static void mstm_clearOutEvents(Mstm* handle) {
}

void mstm_runCycle(Mstm* handle) {
	
	mstm_clearOutEvents(handle);
	
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < MSTM_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++) {
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition]) {
		case Mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown : {
			mstm_react_UserAbsence_UserPresent_UserPresent_TimeoutCountDown(handle);
			break;
		}
		case Mstm_UserAbsence_UserPresent_UserPresent_UserInput : {
			mstm_react_UserAbsence_UserPresent_UserPresent_UserInput(handle);
			break;
		}
		case Mstm_UserAbsence_UserAbsence : {
			mstm_react_UserAbsence_UserAbsence(handle);
			break;
		}
		case Mstm_main_VerifyEEprom : {
			mstm_react_main_VerifyEEprom(handle);
			break;
		}
		case Mstm_main_EEpromVerifiedOk : {
			mstm_react_main_EEpromVerifiedOk(handle);
			break;
		}
		case Mstm_main_EEpromVerifiedCorrupt : {
			mstm_react_main_EEpromVerifiedCorrupt(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
			mstm_react_main_NormalFunction_NormalFunction_StartScreen(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
			mstm_react_main_NormalFunction_NormalFunction_SelectPCB(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Exposure : {
			mstm_react_main_NormalFunction_NormalFunction_Exposure(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Exposing : {
			mstm_react_main_NormalFunction_NormalFunction_Exposing(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
			mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
			mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
			mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
			mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
			mstm_react_main_NormalFunction_NormalFunction_ExposureDone(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
			mstm_react_main_NormalFunction_NormalFunction_ExposurePaused(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
			mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
			mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Statistics(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
			mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
			mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
			mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
			mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
			mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
			mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone(handle);
			break;
		}
		case Mstm_main_UserAbsence : {
			mstm_react_main_UserAbsence(handle);
			break;
		}
		case Mstm_main_PowerDown_PowerDown_PowerDownState : {
			mstm_react_main_PowerDown_PowerDown_PowerDownState(handle);
			break;
		}
		case Mstm_main_PowerDown_PowerDown_Recover : {
			mstm_react_main_PowerDown_PowerDown_Recover(handle);
			break;
		}
		default:
			break;
		}
	}
	
	mstm_clearInEvents(handle);
}

void mstm_raiseTimeEvent(Mstm* handle, sc_eventid evid) {
	if ( ((intptr_t)evid) >= ((intptr_t)&(handle->timeEvents))
		&&  ((intptr_t)evid) < ((intptr_t)&(handle->timeEvents)) + sizeof(MstmTimeEvents)) {
		*(sc_boolean*)evid = bool_true;
	}		
}

sc_boolean mstm_isActive(Mstm* handle, MstmStates state) {
	switch (state) {
		case Mstm_UserAbsence_UserPresent : 
			return (sc_boolean) (handle->stateConfVector[0] >= Mstm_UserAbsence_UserPresent
				&& handle->stateConfVector[0] <= Mstm_UserAbsence_UserPresent_UserPresent_UserInput);
		case Mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown : 
			return (sc_boolean) (handle->stateConfVector[0] == Mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown
			);
		case Mstm_UserAbsence_UserPresent_UserPresent_UserInput : 
			return (sc_boolean) (handle->stateConfVector[0] == Mstm_UserAbsence_UserPresent_UserPresent_UserInput
			);
		case Mstm_UserAbsence_UserAbsence : 
			return (sc_boolean) (handle->stateConfVector[0] == Mstm_UserAbsence_UserAbsence
			);
		case Mstm_main_VerifyEEprom : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_VerifyEEprom
			);
		case Mstm_main_EEpromVerifiedOk : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_EEpromVerifiedOk
			);
		case Mstm_main_EEpromVerifiedCorrupt : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_EEpromVerifiedCorrupt
			);
		case Mstm_main_NormalFunction : 
			return (sc_boolean) (handle->stateConfVector[1] >= Mstm_main_NormalFunction
				&& handle->stateConfVector[1] <= Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone);
		case Mstm_main_NormalFunction_NormalFunction_StartScreen : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_StartScreen
			);
		case Mstm_main_NormalFunction_NormalFunction_SelectPCB : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_SelectPCB
			);
		case Mstm_main_NormalFunction_NormalFunction_Exposure : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Exposure
			);
		case Mstm_main_NormalFunction_NormalFunction_Exposing : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Exposing
			);
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime : 
			return (sc_boolean) (handle->stateConfVector[1] >= Mstm_main_NormalFunction_NormalFunction_CustomExposureTime
				&& handle->stateConfVector[1] <= Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4);
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1
			);
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2
			);
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3
			);
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4
			);
		case Mstm_main_NormalFunction_NormalFunction_ExposureDone : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_ExposureDone
			);
		case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_ExposurePaused
			);
		case Mstm_main_NormalFunction_NormalFunction_Settings : 
			return (sc_boolean) (handle->stateConfVector[1] >= Mstm_main_NormalFunction_NormalFunction_Settings
				&& handle->stateConfVector[1] <= Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault
			);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics
			);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts
			);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion
			);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear : 
			return (sc_boolean) (handle->stateConfVector[1] >= Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear
				&& handle->stateConfVector[1] <= Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear
			);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending
			);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing
			);
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone
			);
		case Mstm_main_UserAbsence : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_UserAbsence
			);
		case Mstm_main_PowerDown : 
			return (sc_boolean) (handle->stateConfVector[1] >= Mstm_main_PowerDown
				&& handle->stateConfVector[1] <= Mstm_main_PowerDown_PowerDown_Recover);
		case Mstm_main_PowerDown_PowerDown_PowerDownState : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_PowerDown_PowerDown_PowerDownState
			);
		case Mstm_main_PowerDown_PowerDown_Recover : 
			return (sc_boolean) (handle->stateConfVector[1] == Mstm_main_PowerDown_PowerDown_Recover
			);
		default: return bool_false;
	}
}

void mstmIface_raise_leftButtonPressed(Mstm* handle) {
	handle->iface.leftButtonPressed_raised = bool_true;
}
void mstmIface_raise_leftButtonReleased(Mstm* handle) {
	handle->iface.leftButtonReleased_raised = bool_true;
}
void mstmIface_raise_rightButtonPressed(Mstm* handle) {
	handle->iface.rightButtonPressed_raised = bool_true;
}
void mstmIface_raise_eepromDataVerifiedOk(Mstm* handle) {
	handle->iface.eepromDataVerifiedOk_raised = bool_true;
}
void mstmIface_raise_eepromDataVerifiedCorrupt(Mstm* handle) {
	handle->iface.eepromDataVerifiedCorrupt_raised = bool_true;
}
void mstmIface_raise_powerDown(Mstm* handle) {
	handle->iface.powerDown_raised = bool_true;
}
void mstmIface_raise_timerTimeout(Mstm* handle) {
	handle->iface.timerTimeout_raised = bool_true;
}
void mstmIface_raise_timerRunning(Mstm* handle) {
	handle->iface.timerRunning_raised = bool_true;
}


sc_integer mstmIface_get_expType(Mstm* handle) {
	return handle->iface.expType;
}
void mstmIface_set_expType(Mstm* handle, sc_integer value) {
	handle->iface.expType = value;
}
sc_integer mstmIface_get_digit(Mstm* handle) {
	return handle->iface.digit;
}
void mstmIface_set_digit(Mstm* handle, sc_integer value) {
	handle->iface.digit = value;
}
sc_boolean mstmIface_get_userAbsence(Mstm* handle) {
	return handle->iface.userAbsence;
}
void mstmIface_set_userAbsence(Mstm* handle, sc_boolean value) {
	handle->iface.userAbsence = value;
}

// implementations of all internal functions

/* Entry action for statechart 'mstm'. */
static void mstm_enact_SequenceImpl(Mstm* handle) {
}

/* Exit action for state 'mstm'. */
static void mstm_exact_SequenceImpl(Mstm* handle) {
}

/* deep enterSequence with history in child NormalFunction */
static void mstm_dhenseq_SequenceImpl(Mstm* handle) {
	/* deep enterSequence with history in child NormalFunction */
	/* Handle deep history entry of NormalFunction */
	switch(handle->historyVector[ 0 ]) {
		case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
			/* enterSequence with history in child StartScreen for leaf StartScreen */
			/* 'default' enter sequence for state StartScreen */
			/* Entry action for state 'StartScreen'. */
			mstmIface_printStartScreen();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[0] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
			/* enterSequence with history in child SelectPCB for leaf SelectPCB */
			/* 'default' enter sequence for state SelectPCB */
			/* Entry action for state 'SelectPCB'. */
			mstmIface_printSelectExpType(handle->iface.expType);
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_SelectPCB;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[0] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Exposure : {
			/* enterSequence with history in child Exposure for leaf Exposure */
			/* 'default' enter sequence for state Exposure */
			/* Entry action for state 'Exposure'. */
			mstmIface_printExposure(handle->iface.expType);
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Exposure;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[0] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Exposing : {
			/* enterSequence with history in child Exposing for leaf Exposing */
			/* 'default' enter sequence for state Exposing */
			/* Entry action for state 'Exposing'. */
			mstmIface_printExposing();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Exposing;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[0] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
			/* enterSequence with history in child CustomExposureTime for leaf Digit1 */
			mstm_dhenseq_main_NormalFunction_NormalFunction_CustomExposureTime_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
			/* enterSequence with history in child CustomExposureTime for leaf Digit2 */
			mstm_dhenseq_main_NormalFunction_NormalFunction_CustomExposureTime_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
			/* enterSequence with history in child CustomExposureTime for leaf Digit3 */
			mstm_dhenseq_main_NormalFunction_NormalFunction_CustomExposureTime_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
			/* enterSequence with history in child CustomExposureTime for leaf Digit4 */
			mstm_dhenseq_main_NormalFunction_NormalFunction_CustomExposureTime_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
			/* enterSequence with history in child ExposureDone for leaf ExposureDone */
			/* 'default' enter sequence for state ExposureDone */
			/* Entry action for state 'ExposureDone'. */
			mstmIface_printExposureDone();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_ExposureDone;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[0] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
			/* enterSequence with history in child ExposurePaused for leaf ExposurePaused */
			/* 'default' enter sequence for state ExposurePaused */
			/* Entry action for state 'ExposurePaused'. */
			mstmIface_printExposurePaused();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_ExposurePaused;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[0] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
			/* enterSequence with history in child Settings for leaf TimersDefault */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
			/* enterSequence with history in child Settings for leaf Statistics */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
			/* enterSequence with history in child Settings for leaf NofStarts */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
			/* enterSequence with history in child Settings for leaf ClearQuestion */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
			/* enterSequence with history in child Settings for leaf ConfirmClear */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
			/* enterSequence with history in child Settings for leaf ClearPending */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
			/* enterSequence with history in child Settings for leaf Clearing */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
			/* enterSequence with history in child Settings for leaf ClearDone */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(handle);
			break;
		}
		default: break;
	}
}

/* deep enterSequence with history in child r1 */
static void mstm_dhenseq_main_NormalFunction_NormalFunction_CustomExposureTime_SequenceImpl(Mstm* handle) {
	/* deep enterSequence with history in child r1 */
	/* Handle deep history entry of r1 */
	switch(handle->historyVector[ 1 ]) {
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
			/* enterSequence with history in child Digit1 for leaf Digit1 */
			/* 'default' enter sequence for state Digit1 */
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[1] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
			/* enterSequence with history in child Digit2 for leaf Digit2 */
			/* 'default' enter sequence for state Digit2 */
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[1] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
			/* enterSequence with history in child Digit3 for leaf Digit3 */
			/* 'default' enter sequence for state Digit3 */
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[1] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
			/* enterSequence with history in child Digit4 for leaf Digit4 */
			/* 'default' enter sequence for state Digit4 */
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[1] = handle->stateConfVector[1];
			break;
		}
		default: break;
	}
}

/* shallow enterSequence with history in child r1 */
static void mstm_shenseq_SequenceImpl(Mstm* handle) {
	/* shallow enterSequence with history in child r1 */
	/* Handle shallow history entry of r1 */
	switch(handle->historyVector[ 1 ]) {
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
			/* 'default' enter sequence for state Digit1 */
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[1] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
			/* 'default' enter sequence for state Digit2 */
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[1] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
			/* 'default' enter sequence for state Digit3 */
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[1] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
			/* 'default' enter sequence for state Digit4 */
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[1] = handle->stateConfVector[1];
			break;
		}
		default: break;
	}
}

/* deep enterSequence with history in child Settings */
static void mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_SequenceImpl(Mstm* handle) {
	/* deep enterSequence with history in child Settings */
	/* Handle deep history entry of Settings */
	switch(handle->historyVector[ 2 ]) {
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
			/* enterSequence with history in child TimersDefault for leaf TimersDefault */
			/* 'default' enter sequence for state TimersDefault */
			/* Entry action for state 'TimersDefault'. */
			mstmIface_printSetTimersDefaultValue();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[2] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
			/* enterSequence with history in child Statistics for leaf Statistics */
			/* 'default' enter sequence for state Statistics */
			/* Entry action for state 'Statistics'. */
			mstmIface_printStatistics();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[2] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
			/* enterSequence with history in child NofStarts for leaf NofStarts */
			/* 'default' enter sequence for state NofStarts */
			/* Entry action for state 'NofStarts'. */
			mstmIface_printNofStarts();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[2] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
			/* enterSequence with history in child ClearQuestion for leaf ClearQuestion */
			/* 'default' enter sequence for state ClearQuestion */
			/* Entry action for state 'ClearQuestion'. */
			mstmIface_printClearStatQuestion();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[2] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
			/* enterSequence with history in child Clear for leaf ConfirmClear */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_Settings_Clear_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
			/* enterSequence with history in child Clear for leaf ClearPending */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_Settings_Clear_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
			/* enterSequence with history in child Clear for leaf Clearing */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_Settings_Clear_SequenceImpl(handle);
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
			/* enterSequence with history in child Clear for leaf ClearDone */
			mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_Settings_Clear_SequenceImpl(handle);
			break;
		}
		default: break;
	}
}

/* deep enterSequence with history in child r1 */
static void mstm_dhenseq_main_NormalFunction_NormalFunction_Settings_Settings_Clear_SequenceImpl(Mstm* handle) {
	/* deep enterSequence with history in child r1 */
	/* Handle deep history entry of r1 */
	switch(handle->historyVector[ 3 ]) {
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
			/* enterSequence with history in child ConfirmClear for leaf ConfirmClear */
			/* 'default' enter sequence for state ConfirmClear */
			/* Entry action for state 'ConfirmClear'. */
			mstmIface_printConfirmClear();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[3] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
			/* enterSequence with history in child ClearPending for leaf ClearPending */
			/* 'default' enter sequence for state ClearPending */
			/* Entry action for state 'ClearPending'. */
			mstm_setTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) , 5 * 1000, bool_false);
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[3] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
			/* enterSequence with history in child Clearing for leaf Clearing */
			/* 'default' enter sequence for state Clearing */
			/* Entry action for state 'Clearing'. */
			mstm_setTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) , 3 * 1000, bool_false);
			mstmIface_printClearing();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[3] = handle->stateConfVector[1];
			break;
		}
		case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
			/* enterSequence with history in child ClearDone for leaf ClearDone */
			/* 'default' enter sequence for state ClearDone */
			/* Entry action for state 'ClearDone'. */
			mstmIface_printClearDone();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[3] = handle->stateConfVector[1];
			break;
		}
		default: break;
	}
}

/* The reactions of state TimeoutCountDown. */
static void mstm_react_UserAbsence_UserPresent_UserPresent_TimeoutCountDown(Mstm* handle) {
	/* The reactions of state TimeoutCountDown. */
	if (handle->iface.leftButtonPressed_raised || handle->iface.rightButtonPressed_raised || handle->iface.timerRunning_raised) { 
		/* Default exit sequence for state TimeoutCountDown */
		handle->stateConfVector[0] = Mstm_last_state;
		handle->stateConfVectorPosition = 0;
		/* Exit action for state 'TimeoutCountDown'. */
		mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown_tev0_raised) );		
		/* 'default' enter sequence for state UserInput */
		handle->stateConfVector[0] = Mstm_UserAbsence_UserPresent_UserPresent_UserInput;
		handle->stateConfVectorPosition = 0;
	}  else {
		if (handle->timeEvents.mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown_tev0_raised) { 
			/* Default exit sequence for state UserPresent */
			/* Default exit sequence for region UserPresent */
			/* Handle exit of all possible states (of UserPresent) at position 0... */
			switch(handle->stateConfVector[ 0 ]) {
				case Mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown : {
					/* Default exit sequence for state TimeoutCountDown */
					handle->stateConfVector[0] = Mstm_last_state;
					handle->stateConfVectorPosition = 0;
					/* Exit action for state 'TimeoutCountDown'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown_tev0_raised) );		
					break;
				}
				case Mstm_UserAbsence_UserPresent_UserPresent_UserInput : {
					/* Default exit sequence for state UserInput */
					handle->stateConfVector[0] = Mstm_last_state;
					handle->stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			handle->iface.userAbsence = bool_true;
			handle->stateConfVector[0] = Mstm_UserAbsence_UserAbsence;
			handle->stateConfVectorPosition = 0;
		} 
	}
}

/* The reactions of state UserInput. */
static void mstm_react_UserAbsence_UserPresent_UserPresent_UserInput(Mstm* handle) {
	/* The reactions of state UserInput. */
}

/* The reactions of state UserAbsence. */
static void mstm_react_UserAbsence_UserAbsence(Mstm* handle) {
	/* The reactions of state UserAbsence. */
	if (handle->iface.leftButtonPressed_raised || handle->iface.rightButtonPressed_raised || handle->iface.timerRunning_raised) { 
		/* Default exit sequence for state UserAbsence */
		handle->stateConfVector[0] = Mstm_last_state;
		handle->stateConfVectorPosition = 0;
		/* Entry action for state 'UserPresent'. */
		handle->iface.userAbsence = bool_false;
		/* 'default' enter sequence for state UserInput */
		handle->stateConfVector[0] = Mstm_UserAbsence_UserPresent_UserPresent_UserInput;
		handle->stateConfVectorPosition = 0;
	} 
}

/* The reactions of state VerifyEEprom. */
static void mstm_react_main_VerifyEEprom(Mstm* handle) {
	/* The reactions of state VerifyEEprom. */
	if (handle->iface.eepromDataVerifiedOk_raised) { 
		/* Default exit sequence for state VerifyEEprom */
		handle->stateConfVector[1] = Mstm_last_state;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for state EEpromVerifiedOk */
		/* Entry action for state 'EEpromVerifiedOk'. */
		mstm_setTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_EEpromVerifiedOk_tev0_raised) , 3 * 1000, bool_false);
		mstmIface_printStartUpCheckOk();
		handle->stateConfVector[1] = Mstm_main_EEpromVerifiedOk;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.eepromDataVerifiedCorrupt_raised) { 
			/* Default exit sequence for state VerifyEEprom */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			/* 'default' enter sequence for state EEpromVerifiedCorrupt */
			/* Entry action for state 'EEpromVerifiedCorrupt'. */
			mstmIface_printStartUpCheckFailed();
			mstmIface_playToneWarning();
			handle->stateConfVector[1] = Mstm_main_EEpromVerifiedCorrupt;
			handle->stateConfVectorPosition = 1;
		} 
	}
}

/* The reactions of state EEpromVerifiedOk. */
static void mstm_react_main_EEpromVerifiedOk(Mstm* handle) {
	/* The reactions of state EEpromVerifiedOk. */
	if (handle->timeEvents.mstm_main_EEpromVerifiedOk_tev0_raised) { 
		/* Default exit sequence for state EEpromVerifiedOk */
		handle->stateConfVector[1] = Mstm_last_state;
		handle->stateConfVectorPosition = 1;
		/* Exit action for state 'EEpromVerifiedOk'. */
		mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_EEpromVerifiedOk_tev0_raised) );		
		/* 'default' enter sequence for state NormalFunction */
		/* 'default' enter sequence for region NormalFunction */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state StartScreen */
		/* Entry action for state 'StartScreen'. */
		mstmIface_printStartScreen();
		handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
		handle->stateConfVectorPosition = 1;
		handle->historyVector[0] = handle->stateConfVector[1];
	}  else {
		if (handle->iface.leftButtonPressed_raised || handle->iface.rightButtonPressed_raised) { 
			/* Default exit sequence for state EEpromVerifiedOk */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'EEpromVerifiedOk'. */
			mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_EEpromVerifiedOk_tev0_raised) );		
			/* 'default' enter sequence for state NormalFunction */
			/* 'default' enter sequence for region NormalFunction */
			/* Default react sequence for initial entry  */
			/* 'default' enter sequence for state StartScreen */
			/* Entry action for state 'StartScreen'. */
			mstmIface_printStartScreen();
			handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
			handle->stateConfVectorPosition = 1;
			handle->historyVector[0] = handle->stateConfVector[1];
		} 
	}
}

/* The reactions of state EEpromVerifiedCorrupt. */
static void mstm_react_main_EEpromVerifiedCorrupt(Mstm* handle) {
	/* The reactions of state EEpromVerifiedCorrupt. */
	if (handle->iface.leftButtonPressed_raised || handle->iface.rightButtonPressed_raised) { 
		/* Default exit sequence for state EEpromVerifiedCorrupt */
		handle->stateConfVector[1] = Mstm_last_state;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for state NormalFunction */
		/* 'default' enter sequence for region NormalFunction */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state StartScreen */
		/* Entry action for state 'StartScreen'. */
		mstmIface_printStartScreen();
		handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
		handle->stateConfVectorPosition = 1;
		handle->historyVector[0] = handle->stateConfVector[1];
	} 
}

/* The reactions of state StartScreen. */
static void mstm_react_main_NormalFunction_NormalFunction_StartScreen(Mstm* handle) {
	/* The reactions of state StartScreen. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for state Settings */
				/* 'default' enter sequence for region Settings */
				/* Default react sequence for initial entry  */
				/* 'default' enter sequence for state TimersDefault */
				/* Entry action for state 'TimersDefault'. */
				mstmIface_printSetTimersDefaultValue();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[2] = handle->stateConfVector[1];
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state SelectPCB */
					/* Entry action for state 'SelectPCB'. */
					mstmIface_printSelectExpType(handle->iface.expType);
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_SelectPCB;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[0] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state SelectPCB. */
static void mstm_react_main_NormalFunction_NormalFunction_SelectPCB(Mstm* handle) {
	/* The reactions of state SelectPCB. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				handle->iface.expType = (handle->iface.expType + 1) % mstmIface_nofExpTypes();
				/* 'default' enter sequence for state SelectPCB */
				/* Entry action for state 'SelectPCB'. */
				mstmIface_printSelectExpType(handle->iface.expType);
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_SelectPCB;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* The reactions of state null. */
					if (handle->iface.expType == mstmIface_nofExpTypes() - 1) { 
						/* 'default' enter sequence for state CustomExposureTime */
						/* 'default' enter sequence for region r1 */
						/* Default react sequence for initial entry  */
						/* 'default' enter sequence for state Digit1 */
						handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1;
						handle->stateConfVectorPosition = 1;
						handle->historyVector[1] = handle->stateConfVector[1];
						handle->historyVector[0] = handle->stateConfVector[1];
					}  else {
						/* 'default' enter sequence for state Exposure */
						/* Entry action for state 'Exposure'. */
						mstmIface_printExposure(handle->iface.expType);
						handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Exposure;
						handle->stateConfVectorPosition = 1;
						handle->historyVector[0] = handle->stateConfVector[1];
					}
				} 
			}
		}
	}
}

/* The reactions of state Exposure. */
static void mstm_react_main_NormalFunction_NormalFunction_Exposure(Mstm* handle) {
	/* The reactions of state Exposure. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for state StartScreen */
				/* Entry action for state 'StartScreen'. */
				mstmIface_printStartScreen();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state Exposing */
					/* Entry action for state 'Exposing'. */
					mstmIface_printExposing();
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Exposing;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[0] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state Exposing. */
static void mstm_react_main_NormalFunction_NormalFunction_Exposing(Mstm* handle) {
	/* The reactions of state Exposing. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for state StartScreen */
				/* Entry action for state 'StartScreen'. */
				mstmIface_printStartScreen();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.timerTimeout_raised) { 
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state ExposureDone */
					/* Entry action for state 'ExposureDone'. */
					mstmIface_printExposureDone();
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_ExposureDone;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[0] = handle->stateConfVector[1];
				}  else {
					if (handle->iface.leftButtonPressed_raised) { 
						/* Default exit sequence for state Exposing */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* 'default' enter sequence for state ExposurePaused */
						/* Entry action for state 'ExposurePaused'. */
						mstmIface_printExposurePaused();
						handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_ExposurePaused;
						handle->stateConfVectorPosition = 1;
						handle->historyVector[0] = handle->stateConfVector[1];
					} 
				}
			}
		}
	}
}

/* The reactions of state Digit1. */
static void mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1(Mstm* handle) {
	/* The reactions of state Digit1. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state CustomExposureTime */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
						/* Default exit sequence for state Digit1 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
						/* Default exit sequence for state Digit2 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
						/* Default exit sequence for state Digit3 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
						/* Default exit sequence for state Digit4 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Digit4'. */
						mstmIface_writeCustomTimer();
						break;
					}
					default: break;
				}
				handle->iface.digit = (handle->iface.digit + 1) % 10;
				/* Default react sequence for shallow history entry Recover */
				/* Enter the region with shallow history */
				if (handle->historyVector[1] != Mstm_last_state) {
					mstm_shenseq_SequenceImpl(handle);
				} 
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[1] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state Digit2. */
static void mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2(Mstm* handle) {
	/* The reactions of state Digit2. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state CustomExposureTime */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
						/* Default exit sequence for state Digit1 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
						/* Default exit sequence for state Digit2 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
						/* Default exit sequence for state Digit3 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
						/* Default exit sequence for state Digit4 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Digit4'. */
						mstmIface_writeCustomTimer();
						break;
					}
					default: break;
				}
				handle->iface.digit = (handle->iface.digit + 1) % 10;
				/* Default react sequence for shallow history entry Recover */
				/* Enter the region with shallow history */
				if (handle->historyVector[1] != Mstm_last_state) {
					mstm_shenseq_SequenceImpl(handle);
				} 
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[1] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state Digit3. */
static void mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3(Mstm* handle) {
	/* The reactions of state Digit3. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state CustomExposureTime */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
						/* Default exit sequence for state Digit1 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
						/* Default exit sequence for state Digit2 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
						/* Default exit sequence for state Digit3 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
						/* Default exit sequence for state Digit4 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Digit4'. */
						mstmIface_writeCustomTimer();
						break;
					}
					default: break;
				}
				handle->iface.digit = (handle->iface.digit + 1) % 10;
				/* Default react sequence for shallow history entry Recover */
				/* Enter the region with shallow history */
				if (handle->historyVector[1] != Mstm_last_state) {
					mstm_shenseq_SequenceImpl(handle);
				} 
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[1] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state Digit4. */
static void mstm_react_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4(Mstm* handle) {
	/* The reactions of state Digit4. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state CustomExposureTime */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
						/* Default exit sequence for state Digit1 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
						/* Default exit sequence for state Digit2 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
						/* Default exit sequence for state Digit3 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
						/* Default exit sequence for state Digit4 */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Digit4'. */
						mstmIface_writeCustomTimer();
						break;
					}
					default: break;
				}
				handle->iface.digit = (handle->iface.digit + 1) % 10;
				/* Default react sequence for shallow history entry Recover */
				/* Enter the region with shallow history */
				if (handle->historyVector[1] != Mstm_last_state) {
					mstm_shenseq_SequenceImpl(handle);
				} 
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state CustomExposureTime */
					/* Default exit sequence for region r1 */
					/* Handle exit of all possible states (of r1) at position 1... */
					switch(handle->stateConfVector[ 1 ]) {
						case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
							/* Default exit sequence for state Digit1 */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
							/* Default exit sequence for state Digit2 */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
							/* Default exit sequence for state Digit3 */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
							/* Default exit sequence for state Digit4 */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							/* Exit action for state 'Digit4'. */
							mstmIface_writeCustomTimer();
							break;
						}
						default: break;
					}
					/* 'default' enter sequence for state Exposure */
					/* Entry action for state 'Exposure'. */
					mstmIface_printExposure(handle->iface.expType);
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Exposure;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[0] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state ExposureDone. */
static void mstm_react_main_NormalFunction_NormalFunction_ExposureDone(Mstm* handle) {
	/* The reactions of state ExposureDone. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for state StartScreen */
				/* Entry action for state 'StartScreen'. */
				mstmIface_printStartScreen();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[0] = handle->stateConfVector[1];
			} 
		}
	}
}

/* The reactions of state ExposurePaused. */
static void mstm_react_main_NormalFunction_NormalFunction_ExposurePaused(Mstm* handle) {
	/* The reactions of state ExposurePaused. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.leftButtonPressed_raised) { 
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for state Exposing */
				/* Entry action for state 'Exposing'. */
				mstmIface_printExposing();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Exposing;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.rightButtonPressed_raised) { 
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state StartScreen */
					/* Entry action for state 'StartScreen'. */
					mstmIface_printStartScreen();
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[0] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state TimersDefault. */
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault(Mstm* handle) {
	/* The reactions of state TimersDefault. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for state Statistics */
				/* Entry action for state 'Statistics'. */
				mstmIface_printStatistics();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[2] = handle->stateConfVector[1];
			} 
		}
	}
}

/* The reactions of state Statistics. */
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Statistics(Mstm* handle) {
	/* The reactions of state Statistics. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.leftButtonPressed_raised) { 
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for state NofStarts */
				/* Entry action for state 'NofStarts'. */
				mstmIface_printNofStarts();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[2] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.rightButtonPressed_raised) { 
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* The reactions of exit default. */
					/* Default exit sequence for state Settings */
					/* Default exit sequence for region Settings */
					/* Handle exit of all possible states (of Settings) at position 1... */
					switch(handle->stateConfVector[ 1 ]) {
						case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
							/* Default exit sequence for state TimersDefault */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
							/* Default exit sequence for state Statistics */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
							/* Default exit sequence for state NofStarts */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
							/* Default exit sequence for state ClearQuestion */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
							/* Default exit sequence for state ConfirmClear */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
							/* Default exit sequence for state ClearPending */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							/* Exit action for state 'ClearPending'. */
							mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
							/* Default exit sequence for state Clearing */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							/* Exit action for state 'Clearing'. */
							mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
							break;
						}
						case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
							/* Default exit sequence for state ClearDone */
							handle->stateConfVector[1] = Mstm_last_state;
							handle->stateConfVectorPosition = 1;
							break;
						}
						default: break;
					}
					/* 'default' enter sequence for state StartScreen */
					/* Entry action for state 'StartScreen'. */
					mstmIface_printStartScreen();
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[0] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state NofStarts. */
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts(Mstm* handle) {
	/* The reactions of state NofStarts. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* 'default' enter sequence for state ClearQuestion */
				/* Entry action for state 'ClearQuestion'. */
				mstmIface_printClearStatQuestion();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[2] = handle->stateConfVector[1];
			} 
		}
	}
}

/* The reactions of state ClearQuestion. */
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion(Mstm* handle) {
	/* The reactions of state ClearQuestion. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* The reactions of exit default. */
				/* Default exit sequence for state Settings */
				/* Default exit sequence for region Settings */
				/* Handle exit of all possible states (of Settings) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
						/* Default exit sequence for state TimersDefault */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
						/* Default exit sequence for state Statistics */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
						/* Default exit sequence for state NofStarts */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
						/* Default exit sequence for state ClearQuestion */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
						/* Default exit sequence for state ConfirmClear */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
						/* Default exit sequence for state ClearPending */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'ClearPending'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
						/* Default exit sequence for state Clearing */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Clearing'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
						/* Default exit sequence for state ClearDone */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* 'default' enter sequence for state StartScreen */
				/* Entry action for state 'StartScreen'. */
				mstmIface_printStartScreen();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[0] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state Clear */
					/* 'default' enter sequence for region r1 */
					/* Default react sequence for initial entry  */
					/* 'default' enter sequence for state ConfirmClear */
					/* Entry action for state 'ConfirmClear'. */
					mstmIface_printConfirmClear();
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[3] = handle->stateConfVector[1];
					handle->historyVector[2] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state ConfirmClear. */
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear(Mstm* handle) {
	/* The reactions of state ConfirmClear. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state Clear */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
						/* Default exit sequence for state ConfirmClear */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
						/* Default exit sequence for state ClearPending */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'ClearPending'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
						/* Default exit sequence for state Clearing */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Clearing'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
						/* Default exit sequence for state ClearDone */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* 'default' enter sequence for state Statistics */
				/* Entry action for state 'Statistics'. */
				mstmIface_printStatistics();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[2] = handle->stateConfVector[1];
			}  else {
				if (handle->iface.leftButtonPressed_raised) { 
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* 'default' enter sequence for state ClearPending */
					/* Entry action for state 'ClearPending'. */
					mstm_setTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) , 5 * 1000, bool_false);
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[3] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state ClearPending. */
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending(Mstm* handle) {
	/* The reactions of state ClearPending. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state Clear */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
						/* Default exit sequence for state ConfirmClear */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
						/* Default exit sequence for state ClearPending */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'ClearPending'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
						/* Default exit sequence for state Clearing */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Clearing'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
						/* Default exit sequence for state ClearDone */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* 'default' enter sequence for state Statistics */
				/* Entry action for state 'Statistics'. */
				mstmIface_printStatistics();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[2] = handle->stateConfVector[1];
			}  else {
				if (handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) { 
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					/* 'default' enter sequence for state Clearing */
					/* Entry action for state 'Clearing'. */
					mstm_setTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) , 3 * 1000, bool_false);
					mstmIface_printClearing();
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[3] = handle->stateConfVector[1];
				}  else {
					if (handle->iface.leftButtonReleased_raised) { 
						/* Default exit sequence for state ClearPending */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'ClearPending'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
						/* 'default' enter sequence for state ConfirmClear */
						/* Entry action for state 'ConfirmClear'. */
						mstmIface_printConfirmClear();
						handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear;
						handle->stateConfVectorPosition = 1;
						handle->historyVector[3] = handle->stateConfVector[1];
					} 
				}
			}
		}
	}
}

/* The reactions of state Clearing. */
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing(Mstm* handle) {
	/* The reactions of state Clearing. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state Clear */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
						/* Default exit sequence for state ConfirmClear */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
						/* Default exit sequence for state ClearPending */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'ClearPending'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
						/* Default exit sequence for state Clearing */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Clearing'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
						/* Default exit sequence for state ClearDone */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* 'default' enter sequence for state Statistics */
				/* Entry action for state 'Statistics'. */
				mstmIface_printStatistics();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[2] = handle->stateConfVector[1];
			}  else {
				if (handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) { 
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					/* 'default' enter sequence for state ClearDone */
					/* Entry action for state 'ClearDone'. */
					mstmIface_printClearDone();
					handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone;
					handle->stateConfVectorPosition = 1;
					handle->historyVector[3] = handle->stateConfVector[1];
				} 
			}
		}
	}
}

/* The reactions of state ClearDone. */
static void mstm_react_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone(Mstm* handle) {
	/* The reactions of state ClearDone. */
	if (handle->iface.powerDown_raised) { 
		/* Default exit sequence for state NormalFunction */
		/* Default exit sequence for region NormalFunction */
		/* Handle exit of all possible states (of NormalFunction) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
				/* Default exit sequence for state StartScreen */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
				/* Default exit sequence for state SelectPCB */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposure : {
				/* Default exit sequence for state Exposure */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Exposing : {
				/* Default exit sequence for state Exposing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
				/* Default exit sequence for state Digit1 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
				/* Default exit sequence for state Digit2 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
				/* Default exit sequence for state Digit3 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
				/* Default exit sequence for state Digit4 */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Digit4'. */
				mstmIface_writeCustomTimer();
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
				/* Default exit sequence for state ExposureDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
				/* Default exit sequence for state ExposurePaused */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
				/* Default exit sequence for state TimersDefault */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
				/* Default exit sequence for state Statistics */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
				/* Default exit sequence for state NofStarts */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
				/* Default exit sequence for state ClearQuestion */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
				/* Default exit sequence for state ConfirmClear */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
				/* Default exit sequence for state ClearPending */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'ClearPending'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
				/* Default exit sequence for state Clearing */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Clearing'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
				break;
			}
			case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
				/* Default exit sequence for state ClearDone */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state PowerDown */
		/* 'default' enter sequence for region PowerDown */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
		handle->stateConfVectorPosition = 1;
	}  else {
		if (handle->iface.userAbsence) { 
			/* Default exit sequence for state NormalFunction */
			/* Default exit sequence for region NormalFunction */
			/* Handle exit of all possible states (of NormalFunction) at position 1... */
			switch(handle->stateConfVector[ 1 ]) {
				case Mstm_main_NormalFunction_NormalFunction_StartScreen : {
					/* Default exit sequence for state StartScreen */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_SelectPCB : {
					/* Default exit sequence for state SelectPCB */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposure : {
					/* Default exit sequence for state Exposure */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Exposing : {
					/* Default exit sequence for state Exposing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1 : {
					/* Default exit sequence for state Digit1 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2 : {
					/* Default exit sequence for state Digit2 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3 : {
					/* Default exit sequence for state Digit3 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4 : {
					/* Default exit sequence for state Digit4 */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Digit4'. */
					mstmIface_writeCustomTimer();
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposureDone : {
					/* Default exit sequence for state ExposureDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_ExposurePaused : {
					/* Default exit sequence for state ExposurePaused */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault : {
					/* Default exit sequence for state TimersDefault */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics : {
					/* Default exit sequence for state Statistics */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts : {
					/* Default exit sequence for state NofStarts */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion : {
					/* Default exit sequence for state ClearQuestion */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
					/* Default exit sequence for state ConfirmClear */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
					/* Default exit sequence for state ClearPending */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'ClearPending'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
					/* Default exit sequence for state Clearing */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					/* Exit action for state 'Clearing'. */
					mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
					break;
				}
				case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
					/* Default exit sequence for state ClearDone */
					handle->stateConfVector[1] = Mstm_last_state;
					handle->stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* 'default' enter sequence for state UserAbsence */
			/* Entry action for state 'UserAbsence'. */
			mstmIface_printStartUpCheckOk();
			handle->stateConfVector[1] = Mstm_main_UserAbsence;
			handle->stateConfVectorPosition = 1;
		}  else {
			if (handle->iface.rightButtonPressed_raised) { 
				/* Default exit sequence for state Clear */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(handle->stateConfVector[ 1 ]) {
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear : {
						/* Default exit sequence for state ConfirmClear */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending : {
						/* Default exit sequence for state ClearPending */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'ClearPending'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing : {
						/* Default exit sequence for state Clearing */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						/* Exit action for state 'Clearing'. */
						mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised) );		
						break;
					}
					case Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone : {
						/* Default exit sequence for state ClearDone */
						handle->stateConfVector[1] = Mstm_last_state;
						handle->stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* 'default' enter sequence for state Statistics */
				/* Entry action for state 'Statistics'. */
				mstmIface_printStatistics();
				handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics;
				handle->stateConfVectorPosition = 1;
				handle->historyVector[2] = handle->stateConfVector[1];
			}  else {
			}
		}
	}
}

/* The reactions of state UserAbsence. */
static void mstm_react_main_UserAbsence(Mstm* handle) {
	/* The reactions of state UserAbsence. */
	if (! handle->iface.userAbsence) { 
		/* Default exit sequence for state UserAbsence */
		handle->stateConfVector[1] = Mstm_last_state;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for state NormalFunction */
		/* 'default' enter sequence for region NormalFunction */
		/* Default react sequence for initial entry  */
		/* 'default' enter sequence for state StartScreen */
		/* Entry action for state 'StartScreen'. */
		mstmIface_printStartScreen();
		handle->stateConfVector[1] = Mstm_main_NormalFunction_NormalFunction_StartScreen;
		handle->stateConfVectorPosition = 1;
		handle->historyVector[0] = handle->stateConfVector[1];
	} 
}

/* The reactions of state PowerDownState. */
static void mstm_react_main_PowerDown_PowerDown_PowerDownState(Mstm* handle) {
	/* The reactions of state PowerDownState. */
	if (! handle->iface.powerDown_raised) { 
		/* Default exit sequence for state PowerDownState */
		handle->stateConfVector[1] = Mstm_last_state;
		handle->stateConfVectorPosition = 1;
		/* 'default' enter sequence for state Recover */
		/* Entry action for state 'Recover'. */
		mstm_setTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_PowerDown_PowerDown_Recover_tev0_raised) , 1 * 1000, bool_false);
		handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_Recover;
		handle->stateConfVectorPosition = 1;
	} 
}

/* The reactions of state Recover. */
static void mstm_react_main_PowerDown_PowerDown_Recover(Mstm* handle) {
	/* The reactions of state Recover. */
	if (handle->timeEvents.mstm_main_PowerDown_PowerDown_Recover_tev0_raised) { 
		/* Default exit sequence for state PowerDown */
		/* Default exit sequence for region PowerDown */
		/* Handle exit of all possible states (of PowerDown) at position 1... */
		switch(handle->stateConfVector[ 1 ]) {
			case Mstm_main_PowerDown_PowerDown_PowerDownState : {
				/* Default exit sequence for state PowerDownState */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				break;
			}
			case Mstm_main_PowerDown_PowerDown_Recover : {
				/* Default exit sequence for state Recover */
				handle->stateConfVector[1] = Mstm_last_state;
				handle->stateConfVectorPosition = 1;
				/* Exit action for state 'Recover'. */
				mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_PowerDown_PowerDown_Recover_tev0_raised) );		
				break;
			}
			default: break;
		}
		/* 'entry_recover0' enter sequence for state NormalFunction */
		/* 'entry_recover0' enter sequence for region NormalFunction */
		/* Default react sequence for initial entry entry_recover0 */
		/* Default react sequence for deep history entry Recover */
		/* Enter the region with deep history */
		if (handle->historyVector[0] != Mstm_last_state) {
			mstm_dhenseq_SequenceImpl(handle);
		} 
	}  else {
		if (handle->iface.powerDown_raised) { 
			/* Default exit sequence for state Recover */
			handle->stateConfVector[1] = Mstm_last_state;
			handle->stateConfVectorPosition = 1;
			/* Exit action for state 'Recover'. */
			mstm_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.mstm_main_PowerDown_PowerDown_Recover_tev0_raised) );		
			/* 'default' enter sequence for state PowerDownState */
			handle->stateConfVector[1] = Mstm_main_PowerDown_PowerDown_PowerDownState;
			handle->stateConfVectorPosition = 1;
		} 
	}
}


