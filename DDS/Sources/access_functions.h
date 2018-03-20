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

_task_id dd_tcreate(uint32_t, uint32_t);
uint32_t dd_delete(_task_id);
uint32_t dd_return_active_list(TASK_LIST_PTR *);
uint32_t dd_return_overdue_list(TASK_LIST_PTR *);

#endif /* SOURCES_ACCESS_FUNCTIONS_H_ */
