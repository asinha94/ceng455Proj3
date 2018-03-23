/* ###################################################################
**     Filename    : os_tasks.c
**     Project     : DDS
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-03-15, 17:12, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         task_dd - void task_dd(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : task_dd
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/

static TASK_LIST_PTR task_list_head = NULL;
static OTASK_LIST_PTR otask_list_head = NULL;

bool remove_from_task_list(_task_id tid, bool overdue) {
	TASK_LIST_PTR task_list_head_temp = task_list_head;
	//check current tasks in task_list

	while(task_list_head_temp != NULL){
		//if tid is = to
		if(task_list_head_temp->tid == tid){
			//remove the task from the list
			task_list_head_temp->next->prev = task_list_head_temp->prev;
			task_list_head_temp->prev->next = task_list_head_temp->next;
			_task_destroy(tid);

			if (overdue) {
				task_list_head_temp->next = otask_list_head;
				task_list_head_temp->prev = NULL;
				otask_list_head = task_list_head_temp;
			} else {
				_mem_free(task_list_head_temp);
			}

			return TRUE;
		}
		//traverse to next node in linked list
		task_list_head_temp = task_list_head_temp->next;
	}
	return FALSE;
}


void task_dd(os_task_param_t task_init_data)
{
	//initialize Message Queues for Various function calls
	_queue_id dd_create_queue = _msgq_open(CREATE_QUEUE_ID, 0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] dd_create_queue queue not created. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	_queue_id dd_delete_queue = _msgq_open(DELETE_QUEUE_ID, 0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] dd_delete_queue queue not created. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	_queue_id dd_return_active_queue = _msgq_open(RETURN_ACTIVE_QUEUE_ID, 0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] dd_return_active_queue queue not created. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	_queue_id dd_return_overdue_queue = _msgq_open(RETURN_OVERDUE_QUEUE_ID, 0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] dd_return_overdue_queue queue not created. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	_queue_id dd_task_finished_queue = _msgq_open(FINISHED_QUEUE_ID, 0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] dd_task_finished_queue queue not created. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}


	// Initialize Message Pools
	dd_create_pool = _msgpool_create(sizeof(CREATE_REQUEST), CREATE_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] Couldn't create task_create_msg_pool. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	dd_delete_pool = _msgpool_create(sizeof(DELETE_REQUEST), DELETE_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] Couldn't create task_delete_msg_pool. Error: 0x%x", _task_get_id());
		_task_set_error(MQX_OK);
		_task_block();
	}

	dd_return_pool = _msgpool_create(sizeof(RETURN_REQUEST), RETURN_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] Couldn't create task_return_msg_pool. Error: 0x%x", _task_get_id());
		_task_set_error(MQX_OK);
		_task_block();
	}

	dd_finished_pool = _msgpool_create(sizeof(TASK_FINISHED), FINISHED_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] Couldn't create task_finished_msg_pool. Error: 0x%x", _task_get_id());
		_task_set_error(MQX_OK);
		_task_block();
	}

  
#ifdef PEX_USE_RTOS
  	while (1) {
#endif


		// Msg counts for any of the queues
		int dd_create_queue_count = 0;
		int dd_delete_queue_count = 0;
		int dd_return_active_queue_count = 0;
		int dd_return_overdue_queue_count = 0;
		int dd_task_finished_queue_count = 0;

		// Get the Counts for any of the queueus
		dd_create_queue_count = _msgq_get_count(dd_create_queue);
		if(dd_create_queue_count == 0 && _task_get_error() != MQX_EOK){
			printf("\r\n[%d] Failed to get count of create queue. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
		}

		dd_delete_queue_count = _msgq_get_count(dd_delete_queue);
		if(dd_delete_queue_count == 0 && _task_get_error() != MQX_EOK){
			printf("\r\n[%d] Failed to get count of delete queue. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
		}

		dd_return_active_queue_count = _msgq_get_count(dd_return_active_queue);
		if(dd_return_active_queue_count == 0 && _task_get_error() != MQX_EOK){
			printf("\r\n[%d] Failed to get count of return active task queue. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
		}

		dd_return_overdue_queue_count = _msgq_get_count(dd_return_overdue_queue);
		if(_task_get_error() != MQX_EOK){
			printf("\r\n[%d] Failed to get count of return overdue task queue. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
		}

		dd_task_finished_queue_count = _msgq_get_count(dd_task_finished_queue);
		if(_task_get_error() != MQX_EOK){
			printf("\r\n[%d] Failed to get count of task finished queue. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
		}

		//Sort task schedule linked list based off inputs from user task
		if(dd_create_queue_count > 0){

			CREATE_REQUEST_PTR create_request = _msgq_receive(dd_create_queue,0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_create_queue message receive error");
			}

			TASK_LIST_PTR task_list_head_temp = task_list_head;
			//check current tasks in task_list

			//first time something needs to be scheduled
			if(task_list_head_temp == NULL){
				TASK_LIST_PTR task_list_new = _mem_alloc(sizeof(TASK_LIST));

				task_list_new->tid = create_request->tid;
				task_list_new->deadline = create_request->deadline;
				task_list_new->task_type = create_request->task_type;
				task_list_new->creation_time = create_request->creation_time;
				task_list_new->next = NULL;
				task_list_new->prev = NULL;
				task_list_head = task_list_new;
			}

			while(task_list_head_temp != NULL){
				//if deadline is less than
				if(task_list_head_temp->deadline > create_request->deadline){

					TASK_LIST_PTR task_list_new = _mem_alloc(sizeof(TASK_LIST));

					task_list_new->tid = create_request->tid;
					task_list_new->deadline = create_request->deadline;
					task_list_new->task_type = create_request->task_type;
					task_list_new->creation_time = create_request->creation_time;
					task_list_new->next = task_list_head_temp;
					task_list_new->prev = task_list_head_temp->prev;
					task_list_head_temp->prev = task_list_new;
					break;
				}
				//if deadline is equal to
				else if(task_list_head_temp->deadline == create_request->deadline){

					TASK_LIST_PTR task_list_new = _mem_alloc(sizeof(TASK_LIST));

					task_list_new->tid = create_request->tid;
					task_list_new->deadline = create_request->deadline;
					task_list_new->task_type = create_request->task_type;
					task_list_new->creation_time = create_request->creation_time;
					task_list_new->next = task_list_head_temp->next;
					task_list_new->prev = task_list_head_temp;
					task_list_head_temp->next = task_list_new;
					break;

				}
				//traverse to next node in linked list
				task_list_head_temp = task_list_head_temp->next;
			}

		}


		if (task_list_head != NULL) {
			TIME_STRUCT current_time;
			_time_get(&current_time);
			MQX_TICK_STRUCT rel_deadline_in_ticks;
			rel_deadline_in_ticks.HW_TICKS = task_list_head->deadline;
			TIME_STRUCT rel_deadline_in_ms;
			uint32_t relative_deadline = _ticks_to_time(&rel_deadline_in_ticks, &rel_deadline_in_ms);
			uint32_t absolute_deadline_in_ms = task_list_head->creation_time.MILLISECONDS + rel_deadline_in_ms.MILLISECONDS;
			if (absolute_deadline_in_ms > current_time.MILLISECONDS) {
				remove_from_task_list(task_list_head->tid, 1);
			}
		}


		//traverse and delete the relevant tasks from the linked list
		if(dd_delete_queue_count > 0){

			DELETE_REQUEST_PTR delete_request = _msgq_receive(dd_delete_queue,0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_delete_queue message receive error");
			}

			remove_from_task_list(delete_request->tid, 0);

		}

		while (dd_task_finished_queue_count > 0) {
			TASK_FINISHED_PTR finish_request = _msgq_receive(dd_task_finished_queue,0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_delete_queue message receive error");
				break;
			}

			remove_from_task_list(finish_request->tid, 1);
			dd_task_finished_queue_count--;
		}

		//Set the priority of the leading task
		if(dd_create_queue_count > 0){

					TASK_LIST_PTR task_list_head_temp = task_list_head;
					//check current tasks in task_list

					int priority = TASK_STARTING_PRIORITY;
					//increase priority of task at head
					while(task_list_head_temp != NULL){
						_mqx_uint_ptr prev_priority;
						_task_get_priority(task_list_head_temp->tid, prev_priority);
						_task_set_priority(task_list_head_temp->tid, priority, prev_priority);
						task_list_head_temp = task_list_head_temp->next;
						priority++;
					}
		}

		//traverse and delete the relevant tasks from the linked list
		if(dd_return_active_queue_count > 0){
			RETURN_REQUEST_PTR return_active_request = _msgq_receive(dd_return_active_queue,0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_delete_queue message receive error");
			}

			return_active_request->list = task_list_head;
			_msgq_send(return_active_request);
			if(_task_get_error() != MQX_EOK){
				printf("dd_return_active_queue message send error");
			}

		}

		//traverse and delete the relevant tasks from the linked list
		if(dd_return_overdue_queue_count > 0){

			RETURN_REQUEST_PTR return_overdue_request = _msgq_receive(dd_return_overdue_queue,0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_overdue_queue message receive error");
			}
			
			return_overdue_request->list = otask_list_head;
			_msgq_send(return_overdue_request);
			if(_task_get_error() != MQX_EOK){
				printf("dd_return_overdue_queue message send error");
			}

		}

#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : monitor_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void monitor_task(os_task_param_t task_init_data)
{
	uint32_t timer = 0;
	uint32_t timerStep = 200000;

#ifdef PEX_USE_RTOS
  while (1) {
#endif
	 _time_delay_ticks(1);
	 timer++;

	 if (timer == timerStep){
		 MQX_TICK_STRUCT elapsed_time;
		 _time_get_elapsed_ticks(&elapsed_time);
		 float utilization = ((float)timer / (float)elapsed_time.HW_TICKS)*100.00;
		 printf("CPU Utilization = %f %%", utilization);
		 timerStep += timerStep;
	 }

    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : p_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void p_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    
    
    OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
    
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
