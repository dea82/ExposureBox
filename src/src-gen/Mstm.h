
#ifndef MSTM_H_
#define MSTM_H_

#include "sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'mstm'.
*/

//! enumeration of all states 
typedef enum {
	Mstm_UserAbsence_UserPresent,
	Mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown,
	Mstm_UserAbsence_UserPresent_UserPresent_UserInput,
	Mstm_UserAbsence_UserAbsence,
	Mstm_main_VerifyEEprom,
	Mstm_main_EEpromVerifiedOk,
	Mstm_main_EEpromVerifiedCorrupt,
	Mstm_main_NormalFunction,
	Mstm_main_NormalFunction_NormalFunction_StartScreen,
	Mstm_main_NormalFunction_NormalFunction_SelectPCB,
	Mstm_main_NormalFunction_NormalFunction_Exposure,
	Mstm_main_NormalFunction_NormalFunction_Exposing,
	Mstm_main_NormalFunction_NormalFunction_CustomExposureTime,
	Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit1,
	Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit2,
	Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit3,
	Mstm_main_NormalFunction_NormalFunction_CustomExposureTime_r1_Digit4,
	Mstm_main_NormalFunction_NormalFunction_ExposureDone,
	Mstm_main_NormalFunction_NormalFunction_ExposurePaused,
	Mstm_main_NormalFunction_NormalFunction_Settings,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_TimersDefault,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Statistics,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_NofStarts,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_ClearQuestion,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ConfirmClear,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing,
	Mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearDone,
	Mstm_main_UserAbsence,
	Mstm_main_PowerDown,
	Mstm_main_PowerDown_PowerDown_PowerDownState,
	Mstm_main_PowerDown_PowerDown_Recover,
	Mstm_last_state
} MstmStates;

//! Type definition of the data structure for the MstmIface interface scope.
typedef struct {
	sc_boolean leftButtonPressed_raised;
	sc_boolean leftButtonReleased_raised;
	sc_boolean rightButtonPressed_raised;
	sc_boolean eepromDataVerifiedOk_raised;
	sc_boolean eepromDataVerifiedCorrupt_raised;
	sc_boolean powerDown_raised;
	sc_boolean timerTimeout_raised;
	sc_boolean timerRunning_raised;
	sc_integer expType;
	sc_integer digit;
	sc_boolean userAbsence;
} MstmIface;

//! Type definition of the data structure for the MstmTimeEvents interface scope.
typedef struct {
	sc_boolean mstm_UserAbsence_UserPresent_UserPresent_TimeoutCountDown_tev0_raised;
	sc_boolean mstm_main_EEpromVerifiedOk_tev0_raised;
	sc_boolean mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_ClearPending_tev0_raised;
	sc_boolean mstm_main_NormalFunction_NormalFunction_Settings_Settings_Clear_r1_Clearing_tev0_raised;
	sc_boolean mstm_main_PowerDown_PowerDown_Recover_tev0_raised;
} MstmTimeEvents;


//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
#define MSTM_MAX_ORTHOGONAL_STATES 2
//! dimension of the state configuration vector for history states
#define MSTM_MAX_HISTORY_STATES 4

/*! Type definition of the data structure for the Mstm state machine.
This data structure has to be allocated by the client code. */
typedef struct {
	MstmStates stateConfVector[MSTM_MAX_ORTHOGONAL_STATES];
	MstmStates historyVector[MSTM_MAX_HISTORY_STATES];
	sc_ushort stateConfVectorPosition; 
	
	MstmIface iface;
	MstmTimeEvents timeEvents;
} Mstm;

/*! Initializes the Mstm state machine data structures. Must be called before first usage.*/
extern void mstm_init(Mstm* handle);

/*! Activates the state machine */
extern void mstm_enter(Mstm* handle);

/*! Deactivates the state machine */
extern void mstm_exit(Mstm* handle);

/*! Performs a 'run to completion' step. */
extern void mstm_runCycle(Mstm* handle);

/*! Raises a time event. */
extern void mstm_raiseTimeEvent(Mstm* handle, sc_eventid evid);

/*! Raises the in event 'leftButtonPressed' that is defined in the default interface scope. */ 
extern void mstmIface_raise_leftButtonPressed(Mstm* handle);

/*! Raises the in event 'leftButtonReleased' that is defined in the default interface scope. */ 
extern void mstmIface_raise_leftButtonReleased(Mstm* handle);

/*! Raises the in event 'rightButtonPressed' that is defined in the default interface scope. */ 
extern void mstmIface_raise_rightButtonPressed(Mstm* handle);

/*! Raises the in event 'eepromDataVerifiedOk' that is defined in the default interface scope. */ 
extern void mstmIface_raise_eepromDataVerifiedOk(Mstm* handle);

/*! Raises the in event 'eepromDataVerifiedCorrupt' that is defined in the default interface scope. */ 
extern void mstmIface_raise_eepromDataVerifiedCorrupt(Mstm* handle);

/*! Raises the in event 'powerDown' that is defined in the default interface scope. */ 
extern void mstmIface_raise_powerDown(Mstm* handle);

/*! Raises the in event 'timerTimeout' that is defined in the default interface scope. */ 
extern void mstmIface_raise_timerTimeout(Mstm* handle);

/*! Raises the in event 'timerRunning' that is defined in the default interface scope. */ 
extern void mstmIface_raise_timerRunning(Mstm* handle);

/*! Gets the value of the variable 'expType' that is defined in the default interface scope. */ 
extern sc_integer mstmIface_get_expType(Mstm* handle);
/*! Sets the value of the variable 'expType' that is defined in the default interface scope. */ 
extern void mstmIface_set_expType(Mstm* handle, sc_integer value);
/*! Gets the value of the variable 'digit' that is defined in the default interface scope. */ 
extern sc_integer mstmIface_get_digit(Mstm* handle);
/*! Sets the value of the variable 'digit' that is defined in the default interface scope. */ 
extern void mstmIface_set_digit(Mstm* handle, sc_integer value);
/*! Gets the value of the variable 'userAbsence' that is defined in the default interface scope. */ 
extern sc_boolean mstmIface_get_userAbsence(Mstm* handle);
/*! Sets the value of the variable 'userAbsence' that is defined in the default interface scope. */ 
extern void mstmIface_set_userAbsence(Mstm* handle, sc_boolean value);


/*! Checks if the specified state is active. */
extern sc_boolean mstm_isActive(Mstm* handle, MstmStates state);

#ifdef __cplusplus
}
#endif 

#endif /* MSTM_H_ */
