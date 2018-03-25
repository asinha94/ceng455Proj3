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
int count = 0;

bool remove_from_task_list(_task_id tid, bool overdue) {
	printf("\r\n Remove From Task List ID: %i bool: %i \r\n", tid, overdue);
	TASK_LIST_PTR task_list_head_temp = task_list_head;
	//count--;
	while(task_list_head_temp != NULL){
		//if tid is = to
		if(task_list_head_temp->tid == tid){
			//remove the task from the list

			//CASE 1: Only 1 node in linked list
			if (task_list_head_temp->next == NULL && task_list_head_temp->prev == NULL) {
				task_list_head = NULL;
				break;
			}

			//CASE 2: Removing last item in linked list
			if (task_list_head_temp->next == NULL && task_list_head_temp->prev != NULL) {
				task_list_head_temp->prev->next = NULL;
				break;
			}

			//CASE 3: Removing First item in linked list
			if (task_list_head_temp->next != NULL && task_list_head_temp->prev == NULL) {
				task_list_head_temp->next->prev = NULL;
				task_list_head = task_list_head_temp->next;
				break;
			}


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

	dd_create_or_finished_pool = _msgpool_create(sizeof(CREATE_REQUEST), CREATE_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] Couldn't create task_create_msg_pool. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}


	dd_return_or_delete_pool = _msgpool_create(sizeof(RETURN_REQUEST), RETURN_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] Couldn't create task_return_msg_pool. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	/*
	dd_delete_pool = _msgpool_create(sizeof(DELETE_REQUEST), DELETE_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] Couldn't create task_delete_msg_pool. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	dd_finished_pool = _msgpool_create(sizeof(TASK_FINISHED), FINISHED_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] Couldn't create task_finished_msg_pool. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}
	*/
  
#ifdef PEX_USE_RTOS
  	while (1) {
#endif

  		//linked list counter

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


		printf("Create Queue Count: %i \n\r", dd_create_queue_count);



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

			count++;
			printf("\r\nCreate LL Count: %i\r\n", count);


			//printf("1_1_1 \r\n");
			CREATE_REQUEST_PTR create_request = _msgq_receive(dd_create_queue, 0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_create_queue message receive error");
			}
			//printf("1_1_2 \r\n");

			TASK_LIST_PTR task_list_head_temp = task_list_head;
			//check current tasks in task_list
			//create_request->HEADER.TARGET_QID = create_request->HEADER.SOURCE_QID;
			//create_request->HEADER.SOURCE_QID = dd_create_queue;


			//first time something needs to be scheduled
			TASK_LIST_PTR task_list_new = _mem_alloc(sizeof(TASK_LIST));
			task_list_new->tid = create_request->tid;
			task_list_new->deadline = create_request->deadline;
			task_list_new->task_type = create_request->task_type;
			task_list_new->creation_time = create_request->creation_time;
			_msg_free(create_request);

			if(task_list_head_temp == NULL){
				task_list_new->next = NULL;
				task_list_new->prev = NULL;
				task_list_head = task_list_new;
			}

		//	printf("1_1_3 \r\n");

			while(task_list_head_temp != NULL){
				//if deadline is less than
				if(task_list_head_temp->deadline > create_request->deadline){
					//last node
					if(task_list_head_temp->next == NULL && task_list_head_temp->prev != NULL){
						task_list_new->next = task_list_head_temp;
						task_list_new->prev = task_list_head_temp->prev;
						task_list_head_temp->prev->next = task_list_new;
						task_list_head_temp->prev = task_list_new;
						break;

					}
					//between nodes
					else if(task_list_head_temp->next != NULL && task_list_head_temp->prev != NULL){
						task_list_new->next = task_list_head_temp;
						task_list_new->prev = task_list_head_temp->prev;
						task_list_head_temp->prev->next = task_list_new;
						task_list_head_temp->prev = task_list_new;
						break;

					}
					//first node
					else if(task_list_head_temp->next != NULL && task_list_head_temp->prev == NULL){
						task_list_new->next = task_list_head_temp;
						task_list_new->prev = NULL;
						task_list_head_temp->prev = task_list_new;
						break;

					}
				}
				//traverse to next node in linked list
				task_list_head_temp = task_list_head_temp->next;
				OSA_TimeDelay(10);
			}

			//printf("1_1_4 \r\n");

		}

		//printf("1_1_5 \r\n");
		if (task_list_head != NULL) {
			TIME_STRUCT current_time;
			_time_get(&current_time);
			/*
			MQX_TICK_STRUCT rel_deadline_in_ticks;
			rel_deadline_in_ticks.HW_TICKS = task_list_head->deadline;
			TIME_STRUCT rel_deadline_in_ms;
			uint32_t relative_deadline = _ticks_to_time(&rel_deadline_in_ticks, &rel_deadline_in_ms);
			printf("1_1_5_1 \r\n");
			uint32_t absolute_deadline_in_ms = task_list_head->creation_time.MILLISECONDS + rel_deadline_in_ms.MILLISECONDS;
			printf("1_1_5_2 \r\n");
			*/
			uint32_t abs_deadline = task_list_head->creation_time.MILLISECONDS +  task_list_head->deadline;
			if (abs_deadline > current_time.MILLISECONDS) {
				remove_from_task_list(task_list_head->tid, 1);
				printf("1_1_5_3 \r\n");
			}
			//printf("1_1_4_4 \r\n");
		}

		//printf("1_1_6 \r\n");


		//traverse and delete the relevant tasks from the linked list
		if(dd_delete_queue_count > 0){

			DELETE_REQUEST_PTR delete_request = _msgq_receive(dd_delete_queue,0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_delete_queue message receive error");
			}

			remove_from_task_list(delete_request->tid, 0);

		}

	//	printf("1_1_7 \r\n");

		while (dd_task_finished_queue_count > 0) {
			TASK_FINISHED_PTR finish_request = _msgq_receive(dd_task_finished_queue,0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_delete_queue message receive error");
				break;
			}

			remove_from_task_list(finish_request->tid, 1);
			_msg_free(finish_request);
			dd_task_finished_queue_count--;
			OSA_TimeDelay(10);
		}

		//printf("1_1_8 \r\n");

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

		//printf("1_1_9 \r\n");

		//traverse and delete the relevant tasks from the linked list
		if(dd_return_active_queue_count > 0){
			printf("\r\n 1_3_1 \r\n");
			RETURN_REQUEST_PTR return_active_request = _msgq_receive(dd_return_active_queue,0);
			if(_task_get_error() != MQX_EOK){
				printf("dd_delete_queue message receive error");
			}
			printf("\r\n 1_3_2 \r\n");
			return_active_request->HEADER.TARGET_QID = return_active_request->HEADER.SOURCE_QID;
			return_active_request->HEADER.SOURCE_QID = dd_return_active_queue;
			return_active_request->list = &task_list_head;
			_msgq_send(return_active_request);
			if(_task_get_error() != MQX_EOK){
				printf("dd_return_active_queue message send error");
			}
			printf("\r\n 1_3_3 \r\n");

		}

		//printf("1_1_10 \r\n");


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

		//printf("1_1_11 \r\n");
		OSA_TimeDelay(100);



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

	_queue_id queue_id = _msgq_open(15, 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to open MSGQ for creating task. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}
	printf("\r\n[%d]Created Queue %d", _task_get_id(), queue_id);

	int tasks = 0;
	TASK_LIST_PTR active_list = NULL;

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    
    
                    /* Example code (for task release) */
	//printf("\r\n Create Periodic Task");

	dd_tcreate(PERIODICTASK_TASK, 10);
	tasks++;
    //printf("1_3 \r\n");
    
	if (tasks == 10) {
		tasks = 0;
		//printf("return Active List FUNCTION ONION");
		dd_return_active_list(&active_list);
		printf("\r\nGot Active List farm");
		int ll_length = 0;
		while (active_list != NULL) {
			printf("\r\nStill iterating");
			ll_length++;
			active_list = active_list->next;
			printf("\r\nTID == %i", active_list->tid);
			OSA_TimeDelay(1);
		}
		printf("\r\nDone iterating");
		printf("\r\n Active Tasks List length = %d", ll_length);
		}

	OSA_TimeDelay(100);
    
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : Periodic_Task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Periodic_Task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */

	//printf("1 \r\n");
	OSA_TimeDelay(task_init_data);
	//printf("\r\n[%d] periodic task", _task_get_id());
	_queue_id dd_task_finished_queue = _msgq_open(_task_get_id(), 0);
	if(_task_get_error() != MQX_EOK){
		printf("\r\n[%d] dd_task_finished_queue queue not created. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	// Send msg to the scheduler
	TASK_FINISHED_PTR msg_send_ptr = (TASK_FINISHED_PTR) _msg_alloc(dd_create_or_finished_pool);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to Allocate task finished msg. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	TIME_STRUCT finish_time;
	_time_get(&finish_time);

	msg_send_ptr->HEADER.SOURCE_QID=dd_task_finished_queue;
	msg_send_ptr->HEADER.TARGET_QID= _msgq_get_id(0, FINISHED_QUEUE_ID);
	msg_send_ptr->tid = _task_get_id();
	msg_send_ptr->finish_time = finish_time;

	_msgq_send(msg_send_ptr);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Failed to finished task msg to DD. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return FALSE;
	}

	printf("\r\n[%d] Finished Periodic Task", _task_get_id());
	OSA_TimeDelay(10);
	return;
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
