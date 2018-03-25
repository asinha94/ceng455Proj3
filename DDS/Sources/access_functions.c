/*
 * access_functions.c
 *
 *  Created on: Mar 18, 2018
 *      Author: asinha
 */

#include "os_tasks.h"
#include "access_functions.h"


_task_id dd_tcreate(uint32_t template_index, uint32_t deadline) {
	TIME_STRUCT creation_time;

	_task_id new_task_id = _task_create(0, template_index, deadline);
	//printf("\r\n[%d] DD_Create TAsk", new_task_id);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to create task. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	_time_get(&creation_time);
	_queue_id queue_id = _msgq_get_id(0, 15);
	printf("\r\n[%d] dd_tcreate using queue %d", _task_get_id(), queue_id);

	// Send msg to the scheduler
	CREATE_REQUEST_PTR msg_send_ptr = (CREATE_REQUEST_PTR) _msg_alloc(dd_create_or_finished_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	printf("\r\n Create Request \r\n ");
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
		_task_set_error(MQX_OK);
		return FALSE;
	}
/*
	int user_msg_count = 0;

	while (TRUE) {
		user_msg_count = _msgq_get_count(queue_id);
		if (user_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get user message count. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
			continue;
		}

		if (user_msg_count > 0) {
			printf("user_msg_count Loop");
			// get msg and do some error handling
			CREATE_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive user message. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				return FALSE;
			}

			// Task set to 0 if we got any errors
			new_task_id = msg_recv_ptr->tid;

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				new_task_id = 0;
			}

			return new_task_id;
		}
		OSA_TimeDelay(10);
	}
	*/
	return new_task_id;

}

uint32_t dd_delete(_task_id delete_task_id) {
	// Because all Task IDs are unique, we can use the Task ID as its QID
	_queue_id queue_id = _msgq_get_id(0, 15);

	// Send msg to the scheduler
	DELETE_REQUEST_PTR msg_send_ptr = (DELETE_REQUEST_PTR) _msg_alloc(dd_return_or_delete_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	msg_send_ptr->HEADER.SIZE = sizeof(DELETE_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = queue_id;
	msg_send_ptr->HEADER.TARGET_QID = _msgq_get_id(0, DELETE_QUEUE_ID);
	msg_send_ptr->tid = delete_task_id;

	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_delete msg. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	int user_msg_count = 0;

	while (TRUE) {
		user_msg_count = _msgq_get_count(queue_id);
		if (user_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get user message count. Error: 0x%x", _task_get_id(), _task_get_error());
			// cross our fingers and hope we don't next time around
			_task_set_error(MQX_OK);
			continue;
		}

		if (user_msg_count > 0) {
			DELETE_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive user message. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				return FALSE;
			}

			// Will be 0 if errors were encountered
			delete_task_id = msg_recv_ptr->tid;

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				delete_task_id = 0;
			}

			_msgq_close(queue_id);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				delete_task_id = 0;
			}

			return delete_task_id;
		}
	}
}

uint32_t dd_return_active_list(TASK_LIST_PTR * active_tasks) {
	printf("\r\n 1_1 \r\n");
	int success = TRUE;
	//_queue_id queue_id = _msgq_open(_task_get_id(), 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for getting active list. Error: 0x%x",  _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	printf("\r\n 1_2 \r\n");
	// Send msg to the scheduler
	RETURN_REQUEST_PTR msg_send_ptr = (RETURN_REQUEST_PTR) _msg_alloc(dd_return_or_delete_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}
	printf("\r\n 1_3 \r\n");
	msg_send_ptr->HEADER.SIZE = sizeof(RETURN_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = _msgq_get_id(0, 15);
	msg_send_ptr->HEADER.TARGET_QID = _msgq_get_id(0, RETURN_ACTIVE_QUEUE_ID);


	printf("\r\n 1_4 \r\n");
	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_delete msg. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	int list_msg_count = 0;
	printf("\r\n 1_5 \r\n");
	while (TRUE) {
		list_msg_count = _msgq_get_count(_msgq_get_id(0, 15));
		if (list_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get active task list message count. Error: 0x%x", _task_get_id(), _task_get_error());
			// cross our fingers and hope we don't next time around
			_task_set_error(MQX_OK);
			continue;
		}

		//printf("\r\nActive Tasks List Count: %d", list_msg_count);
		if (list_msg_count > 0) {
			printf("\r\n 1_1_1_1_1 \r\n");
			RETURN_REQUEST_PTR msg_recv_ptr = _msgq_receive(_msgq_get_id(0, 15), 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive active tasks msg. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				return FALSE;
			}
			printf("\r\n recieved active lists msg");

			*active_tasks = msg_recv_ptr->list;
			if (*active_tasks == NULL) {
				success = FALSE;
			} 

			printf("\r\n Got active lists ptr");
			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				success = FALSE;
			}

			//_msgq_close(queue_id);
		//	if (_task_get_error() != MQX_EOK) {
			//	printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", _task_get_id(), _task_get_error());
			//	_task_set_error(MQX_OK);
		//		success =  FALSE;
		//	}

			printf("\r\n returning active queue count");
			return success;
		}
		//printf("\r\n 1_6 \r\n");
		OSA_TimeDelay(10);

	}
	printf("\r\n 1_7 \r\n");

}

uint32_t dd_return_overdue_list(TASK_LIST_PTR * overdue_tasks) {
	int success = TRUE;
	_queue_id queue_id = _msgq_open(_task_get_id(), 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for getting active list. Error: 0x%x",  _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	// Send msg to the scheduler
	RETURN_REQUEST_PTR msg_send_ptr = (RETURN_REQUEST_PTR) _msg_alloc(dd_return_or_delete_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	msg_send_ptr->HEADER.SIZE = sizeof(RETURN_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = queue_id;
	msg_send_ptr->HEADER.TARGET_QID = _msgq_get_id(0, RETURN_OVERDUE_QUEUE_ID);

	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_delete msg. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	int list_msg_count = 0;

	while (TRUE) {
		list_msg_count = _msgq_get_count(queue_id);
		if (list_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get active task list message count. Error: 0x%x", _task_get_id(), _task_get_error());
			// cross our fingers and hope we don't next time around
			_task_set_error(MQX_OK);
			continue;
		}

		if (list_msg_count > 0) {
			RETURN_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive active tasks msg. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				return FALSE;
			}

			*overdue_tasks = msg_recv_ptr->list;
			if (*overdue_tasks == NULL) {
				success = FALSE;
			} 

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				success = FALSE;
			}

			_msgq_close(queue_id);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", _task_get_id(), _task_get_error());
				_task_set_error(MQX_OK);
				success =  FALSE;
			}

			return success;
		}

	}
}


