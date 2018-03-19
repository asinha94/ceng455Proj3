/*
 * access_functions.c
 *
 *  Created on: Mar 18, 2018
 *      Author: asinha
 */

#include "os_tasks.h"
#include "access_functions.h"


_task_id dd_tcreate(uint32_t template_index, uint32_t deadline) {
	_task_id current_task_id = _task_get_id();

	// Because all Task IDs are unique, we can use the Task ID as its QID
	_queue_id queue_id = _msgq_open(current_task_id, 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for creating task. Error: 0x%x", current_task_id, _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	TIME_STRUCT creation_time;

	//start task
	_task_id new_task_id = 0;
	// do error checking as usual

	_time_get(&creation_time);


	// Send msg to the scheduler
	CREATE_REQUEST_PTR msg_send_ptr = (CREATE_REQUEST_PTR) _msg_alloc(dd_create_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate dd_tcreate msg. Error: 0x%x", current_task_id, _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	msg_send_ptr->HEADER.SIZE = sizeof(CREATE_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = queue_id;
	// TODO: Change this once implemented
	msg_send_ptr->HEADER.TARGET_QID = 0;
	msg_send_ptr->create_task_id = new_task_id;
	//msg_send_ptr->deadline = 0;
	//msg_senf_ptr->creation_time = creation_time;
	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_tcreate msg. Error: 0x%x", current_task_id, _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	int user_msg_count = 0;

	while (TRUE) {
		user_msg_count = _msgq_get_count(queue_id);
		if (user_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get user message count. Error: 0x%x", current_task_id, _task_get_error());
			// cross our fingers and hope we don't next time around
			task_set_error(MQX_OK);
			continue;
		}

		if (user_msg_count > 0) {
			// get msg and do some error handling
			CREATE_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive user message. Error: 0x%x", current_task_id, _task_get_error());
				task_set_error(MQX_OK);
				return FALSE;
			}

			if (msg->received == FALSE) {
				new_task_id = 0;
			}

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", current_task_id, _task_get_error());
				task_set_error(MQX_OK);
				new_task_id = 0;
			}

			_msgq_close(queue_id);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", current_task_id, _task_get_error());
				task_set_error(MQX_OK);
				new_task_id = 0;
			}

			return new_task_id;
		}
	}

}

uint32_t dd_delete(_task_id delete_task_id) {
	// Because all Task IDs are unique, we can use the Task ID as its QID
	_queue_id queue_id = _msgq_open(current_task_id, 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for deleting task. Error: 0x%x", current_task_id, _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	msg_send_ptr->HEADER.SIZE = sizeof(CREATE_REQUEST);
	msg_send_ptr->HEADER.SOURCE_QID = queue_id;
	// TODO: Change this once implemented
	msg_send_ptr->HEADER.TARGET_QID = 0;
	msg_send_ptr->create_task_id = new_task_id;
	//msg_send_ptr->deadline = 0;
	//msg_send_ptr->creation_time = creation_time;
	//msg_send_ptr->CREATE = FALSE;
	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to send dd_tcreate msg. Error: 0x%x", current_task_id, _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	int user_msg_count = 0;

	while (TRUE) {
		user_msg_count = _msgq_get_count(queue_id);
		if (user_msg_count == 0 && _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Failed to get user message count. Error: 0x%x", current_task_id, _task_get_error());
			// cross our fingers and hope we don't next time around
			task_set_error(MQX_OK);
			continue;
		}

		if (user_msg_count > 0) {
			CREATE_REQUEST_PTR msg_recv_ptr = _msgq_receive(queue_id, 0);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to receive user message. Error: 0x%x", current_task_id, _task_get_error());
				task_set_error(MQX_OK);
				return FALSE;
			}

			if (msg->received == FALSE) {
				delete_task_id = 0;
			}

			_msg_free(msg_recv_ptr);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to free user msg. Error: 0x%x", current_task_id, _task_get_error());
				task_set_error(MQX_OK);
				delete_task_id = 0;
			}

			_msgq_close(queue_id);
			if (_task_get_error() != MQX_EOK) {
				printf("\r\n[%d] Failed to Close msgq. Error: 0x%x", current_task_id, _task_get_error());
				task_set_error(MQX_OK);
				delete_task_id = 0;
			}

			return delete_task_id;
		}
	}
}

uint32_t dd_return_active_list(TASK_LIST_PTR * active_tasks) {
	_task_id current_task_id = _task_get_id();

	// Because all Task IDs are unique, we can use the Task ID as its QID
	_queue_id queue_id = _msgq_open(current_task_id, 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for getting active list. Error: 0x%x", current_task_id, _task_get_error());
		task_set_error(MQX_OK);
		return FALSE;
	}

	/*
	 * get msg ptr to active list pool
	 * send request
	 * recieve msg
	 * point list to list in msg
	 * */

}

uint32_t dd_return_overdue_list(OVERDUE_TASK_LIST_PTR * overdue_tasks) {

}


