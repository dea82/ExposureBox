/*
 * sche.h
 *
 *  Created on: 29 maj 2014
 *      Author: andreas
 */

#ifndef SCHE_H_
#define SCHE_H_

typedef void (*task_t)(void);

void Sche_init(void);
void Sche_dispatchTasks(void);
void Sche_addTask(uint8_t id, task_t task, uint16_t period);
void Sche_startScheduler(void);

#endif /* SCHE_H_ */
