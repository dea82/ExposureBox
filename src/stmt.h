/*
 * stmt.h
 *
 *  Created on: 25 okt 2014
 *      Author: andreas
 */

#ifndef STMT_H_
#define STMT_H_

#include "src-gen/Mmstm.h"

typedef void (*raiseEventCallback_t)(Mstm* handle, sc_eventid evid);

void Stmt_setTimer(Mstm* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic, raiseEventCallback_t callback);

void Stmt_unsetTimer(const sc_eventid evid);
void initTimers(void);
void checkTimers(sc_integer deltaT_ms);
#endif /* STMT_H_ */
