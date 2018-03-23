/*
 * access_functions.c
 *
 *  Created on: Mar 18, 2018
 *      Author: asinha
 */

#include "os_tasks.h"
#include "access_functions.h"


_task_id dd_tcreate(uint32_t template_index, uint32_t deadline) {
	_queue_id queue_id = _msgq_open(_task_get_id(), 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for creating task. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	TIME_STRUCT creation_time;

	/*
		Start Task
		Get task id
		do error handling
	*/
	_task_id new_task_id = 42;

	_time_get(&creation_time);

	// Send msg to the scheduler
	CREATE_REQUEST_PTR msg_send_ptr = (CREATE_REQUEST_PTR) _msg_alloc(dd_create_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	msg_send_ptr->HEADER.SIZE = sizeof(CREATE_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = queue_id;
	msg_send_ptr->HEADER.TARGET_QID = _msgq_get_id(0, CREATE_QUEUE_ID);
	msg_send_ptr->tid = new_task_id;
	msg_send_ptr->task_type = new_task_id;
	msg_send_ptr->creation_time = creation_time;
	msg_send_ptr->deadline = deadline;

	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	int user_msg_count = 0;

	while (TRUE) {
		user_msg_count = _msgq_get_count(queue_id);
		if (user_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get user message count. Error: 0x%x", _task_get_id(), _task_get_error());
			task_set_error(MQX_OK);
			continue;
		}

		if (user_msg_count > 0) {
			// get msg and do some error handling
			CREATE_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive user message. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				return FALSE;
			}

			// Task set to 0 if we got any errors
			new_task_id = msg_recv_ptr->tid;

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				new_task_id = 0;
			}

			_msgq_close(queue_id);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				new_task_id = 0;
			}

			return new_task_id;
		}
	}

}

uint32_t dd_delete(_task_id delete_task_id) {
	// Because all Task IDs are unique, we can use the Task ID as its QID
	_queue_id queue_id = _msgq_open(_task_get_id(), 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for deleting task. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	// Send msg to the scheduler
	DELETE_REQUEST_PTR msg_send_ptr = (DELETE_REQUEST_PTR) _msg_alloc(dd_create_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	msg_send_ptr->HEADER.SIZE = sizeof(DELETE_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = queue_id;
	msg_send_ptr->HEADER.TARGET_QID = _msgq_get_id(0, DELETE_QUEUE_ID);
	msg_send_ptr->tid = delete_task_id;

	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_delete msg. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	int user_msg_count = 0;

	while (TRUE) {
		user_msg_count = _msgq_get_count(queue_id);
		if (user_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get user message count. Error: 0x%x", _task_get_id(), _task_get_error());
			// cross our fingers and hope we don't next time around
			task_set_error(MQX_OK);
			continue;
		}

		if (user_msg_count > 0) {
			DELETE_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive user message. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				return FALSE;
			}

			// Will be 0 if errors were encountered
			delete_task_id = msg_recv_ptr->tid;

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				delete_task_id = 0;
			}

			_msgq_close(queue_id);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				delete_task_id = 0;
			}

			return delete_task_id;
		}
	}
}

uint32_t dd_return_active_list(TASK_LIST_PTR * active_tasks) {
	int success = TRUE;
	_queue_id queue_id = _msgq_open(_task_get_id(), 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for getting active list. Error: 0x%x",  _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	// Send msg to the scheduler
	RETURN_REQUEST_PTR msg_send_ptr = (RETURN_REQUEST_PTR) _msg_alloc(dd_create_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	msg_send_ptr->HEADER.SIZE = sizeof(RETURN_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = queue_id;
	msg_send_ptr->HEADER.TARGET_QID = _msgq_get_id(0, RETURN_ACTIVE_QUEUE_ID);

	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_delete msg. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	int list_msg_count = 0;

	while (TRUE) {
		list_msg_count = _msgq_get_count(queue_id);
		if (list_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get active task list message count. Error: 0x%x", _task_get_id(), _task_get_error());
			// cross our fingers and hope we don't next time around
			task_set_error(MQX_OK);
			continue;
		}

		if (list_msg_count > 0) {
			RETURN_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive active tasks msg. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				return FALSE;
			}

			*active_tasks = msg_recv_ptr->list;
			if (*active_tasks == NULL) {
				success = FALSE;
			} 

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				success = FALSE;
			}

			_msgq_close(queue_id);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				success =  FALSE;
			}

			return success;
		}

	}

}

uint32_t dd_return_overdue_list(TASK_LIST_PTR * overdue_tasks) {
	int success = TRUE;
	_queue_id queue_id = _msgq_open(_task_get_id(), 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for getting active list. Error: 0x%x",  _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	// Send msg to the scheduler
	RETURN_REQUEST_PTR msg_send_ptr = (RETURN_REQUEST_PTR) _msg_alloc(dd_create_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	msg_send_ptr->HEADER.SIZE = sizeof(RETURN_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = queue_id;
	msg_send_ptr->HEADER.TARGET_QID = _msgq_get_id(0, RETURN_OVERDUE_QUEUE_ID);

	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_delete msg. Error: 0x%x", _task_get_id(), _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	int list_msg_count = 0;

	while (TRUE) {
		list_msg_count = _msgq_get_count(queue_id);
		if (list_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get active task list message count. Error: 0x%x", _task_get_id(), _task_get_error());
			// cross our fingers and hope we don't next time around
			task_set_error(MQX_OK);
			continue;
		}

		if (list_msg_count > 0) {
			RETURN_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive active tasks msg. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				return FALSE;
			}

			*overdue_tasks = msg_recv_ptr->list;
			if (*overdue_tasks == NULL) {
				success = FALSE;
			} 

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				success = FALSE;
			}

			_msgq_close(queue_id);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", _task_get_id(), _task_get_error());
				task_set_error(MQX_OK);
				success =  FALSE;
			}

			return success;
		}

	}
}


