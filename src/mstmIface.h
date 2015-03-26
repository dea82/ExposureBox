void mstm_setTimer(Mstm* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic);
void mstm_unsetTimer(Mstm* handle, const sc_eventid evid);

sc_integer mstmIface_nofExpTypes(void);
void mstmIface_printSelectExpType(const sc_integer type);

void mstmIface_printStartupCheck(void);
void mstmIface_printStartUpCheckFailed(void);
void mstmIface_printStartUpCheckOk(void);
void mstmIface_playToneWarning(void);
void mstmIface_printStartScreen(void);
void mstmIface_clear(void);

