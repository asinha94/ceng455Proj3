/*
 * access_functions.c
 *
 *  Created on: Mar 18, 2018
 *      Author: asinha
 */

#include "os_tasks.h"
#include "access_functions.h"


_task_id dd_tcreate(uint32_t template_index, uint32_t relative_deadline) {
	TIME_STRUCT creation_time;
	_time_get(&creation_time);
	uint32_t absolute_deadline = creation_time.MILLISECONDS + relative_deadline;

	// Start Task
	_task_id new_task_id = _task_create(0, template_index, absolute_deadline);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to create task. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	printf("\r\n\[%d] Started New Task == %d", _task_get_id(), new_task_id);
	return add_task_to_list(new_task_id, absolute_deadline, relative_deadline);
}

uint32_t dd_delete(_task_id delete_task_id) {
	// Because all Task IDs are unique, we can use the Task ID as its QID
	return remove_from_task_list(delete_task_id);
}

uint32_t dd_return_active_list(TASK_LIST_PTR * active_tasks) {
	return 0;
}

uint32_t dd_return_overdue_list(TASK_LIST_PTR * overdue_tasks) {
	return 0;
}


