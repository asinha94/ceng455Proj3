/*
 * access_functions.c
 *
 *  Created on: Mar 18, 2018
 *      Author: asinha
 */

#include "os_tasks.h"
#include "access_functions.h"


_task_id dd_tcreate(uint32_t template_index, uint32_t relative_deadline, uint32_t execution_time) {
	TIME_STRUCT creation_time;
	_time_get(&creation_time);
	uint32_t absolute_deadline = (creation_time.SECONDS*1000) + creation_time.MILLISECONDS + relative_deadline;
	//printf("\r\n Absolute deadline == %d", absolute_deadline);
	// Start Task
	_task_id new_task_id = _task_create(0, template_index, execution_time);
	OSA_TimeDelay(1);
	if (_task_get_error() != MQX_EOK || new_task_id == 0) {
		printf("\r\n[%d] Failed to create task. Error: 0x%x \r\n", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	//printf("\r\n\[Generator] Started New Task == %d", new_task_id);
	return add_task_to_list(new_task_id, absolute_deadline, relative_deadline);
}

uint32_t dd_delete(_task_id delete_task_id) {
	TIME_STRUCT start_time;
	_time_get(&start_time);
	uint32_t finish_time = (start_time.SECONDS * 1000) + start_time.MILLISECONDS;
	return remove_from_task_list(delete_task_id, finish_time);
}

uint32_t dd_return_active_list(TASK_LIST_PTR * active_tasks) {
	return create_active_list_copy(active_tasks);
}

uint32_t dd_return_overdue_list(TASK_LIST_PTR * overdue_tasks) {
	return 0;
}


