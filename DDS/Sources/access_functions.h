/*
 * access_functions.h
 *
 *  Created on: Mar 18, 2018
 *      Author: asinha
 */

#ifndef SOURCES_ACCESS_FUNCTIONS_H_
#define SOURCES_ACCESS_FUNCTIONS_H_

//#include "os_tasks.h"

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"

#include <message.h>

typedef struct task_list{
	uint32_t tid;
	uint32_t deadline;
	uint32_t task_type;
	uint32_t creation_time;
	struct task_list *next_cell;
	struct task_list *prev_cell;
} TASK_LIST, * TASK_LIST_PTR;

typedef struct overdue_tasks{
	uint32_t tid;
	uint32_t deadline;
	uint32_t task_type;
	uint32_t creation_time;
	struct task_list *next_cell;
	struct task_list *prev_cell;
} OVERDUE_TASK_LIST, * OVERDUE_TASK_LIST_PTR;

_task_id dd_tcreate(uint32_t, uint32_t);
uint32_t dd_delete(_task_id);
uint32_t dd_return_active_list(TASK_LIST_PTR *);
uint32_t dd_return_overdue_list(OVERDUE_TASK_LIST_PTR *);

#endif /* SOURCES_ACCESS_FUNCTIONS_H_ */
