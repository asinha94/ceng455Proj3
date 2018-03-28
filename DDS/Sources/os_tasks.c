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
#include "access_functions.h"
#include "Mutex.h"

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

static MUTEX_STRUCT active_task_mutex;
static MUTEX_STRUCT overdue_task_mutex;

static MUTEX_STRUCT task_exec_mutex;

static TASK_LIST_PTR task_list_head = NULL;
static TASK_LIST_PTR otask_list_head = NULL;

int count = 0;

void print_list(TASK_LIST_PTR iter) {
	printf("\r\nPrinting List");
	while (iter != NULL) {
		printf("->[%d,%d]", iter->tid, iter->absdeadline);
		iter = iter->next;
	}
}

uint32_t create_active_list_copy(TASK_LIST_PTR * ptr_copy){
	int mut_error = _mutex_lock(&active_task_mutex);
	if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Couldn't Lock active task mutex when adding to the list. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return 0;
	}

	TASK_LIST_PTR active_head_iter = task_list_head;
	TASK_LIST_PTR copy_iter = NULL;
	printf("\r\n Starting to copy list");
	while (active_head_iter != NULL) {
		*ptr_copy = _mem_alloc(sizeof(TASK_LIST));
		(*ptr_copy)->tid = active_head_iter->tid;
		printf("\r\n Active Head TID: %u", active_head_iter->tid);
		(*ptr_copy)->absdeadline = active_head_iter->absdeadline;
		(*ptr_copy)->execution_time = active_head_iter->execution_time;
		(*ptr_copy)->task_type = 0;
		(*ptr_copy)->next = NULL;
		if (copy_iter == NULL) {
			copy_iter = *ptr_copy;
		} else {
			copy_iter->next = *ptr_copy;
			copy_iter = copy_iter->next;
		}
		active_head_iter = active_head_iter->next;
	}
	printf("\r\n Finished copy list");

	mut_error = _mutex_unlock(&active_task_mutex);
	if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Couldn't unlock active task mutex when adding to the list. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return 0;
	}

	return TRUE;
}

static void add_task_to_overdue_list(TASK_LIST_PTR task) {
	/*add task to overdue list. doesn't need to be sorted*/
	printf("\r\nI am Overdue");
}

_task_id add_task_to_list(_task_id tid, uint32_t deadline, uint32_t execution_time) {
	int mut_error = _mutex_lock(&active_task_mutex);
	if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Couldn't Lock active task mutex when adding to the list. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return 0;
	}

	TASK_LIST_PTR task_list_new = _mem_alloc(sizeof(TASK_LIST));
	task_list_new->tid = tid;
	task_list_new->absdeadline = deadline;
	task_list_new->execution_time = execution_time;
	task_list_new->task_type = 0;
	task_list_new->next = NULL;
	//task_list_new->prev = NULL;

	TASK_LIST_PTR task_iter = task_list_head;
	TASK_LIST_PTR task_prev = task_list_head;

	if (task_prev == NULL || task_prev->absdeadline >= task_list_new->absdeadline) {
		task_list_new->next = task_list_head;
		task_list_head = task_list_new;
		mut_error = _mutex_unlock(&active_task_mutex);
		if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Couldn't unlock active task mutex when adding to the list. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
			return 0;
		}
		return tid;
	}

	task_iter = task_list_head->next;

	while (task_iter != NULL) {
		if (task_list_new->absdeadline <= task_iter->absdeadline) break;
		task_prev = task_iter;
		task_iter = task_iter->next;
	}

	//task_list_new->prev = task_prev;
	task_list_new->next = task_iter;
	task_prev->next = task_list_new;

	/*
	if (task_iter != NULL) {
		task_iter->prev = task_list_new;
	}
	*/

	mut_error = _mutex_unlock(&active_task_mutex);
	if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Couldn't unlock active task mutex when adding to the list. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return 0;
	}

	return tid;
}

bool remove_from_task_list(_task_id tid) {

	int mut_error = _mutex_lock(&active_task_mutex);
	if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Couldn't Lock active task mutex when removing from list. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return 0;
	}

	bool ret_value = FALSE;

	TASK_LIST_PTR task_iter = task_list_head;
	// Nothing to delete
	if (task_list_head == NULL) {
		mut_error = _mutex_unlock(&active_task_mutex);
		if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Couldn't unlock active task mutex when nothing to remove from list. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
			return ret_value;
		}
		return ret_value;
	}

	TASK_LIST_PTR task_iter_prev = NULL;

	while (task_iter->tid != tid && task_iter->next != NULL) {
		task_iter_prev = task_iter;
		task_iter = task_iter->next;
	}

	if (task_iter->tid == tid) {
		if (task_iter_prev != NULL) {
			task_iter_prev->next = task_iter->next;
		} else {
			task_list_head = task_iter->next;
		}
		ret_value = TRUE;
		/* Add to overdue list here if necessary*/
		_mem_free(task_iter);
	}

	mut_error = _mutex_unlock(&active_task_mutex);
	if (mut_error != MQX_EOK && _task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Couldn't unlock active task mutex when finished removing from the list. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return 0;
	}

	return ret_value;
}


void task_dd(os_task_param_t task_init_data)
{
	printf("starting task DD");

	// Initialize Mutexes to default values
	MUTEX_ATTR_STRUCT mutextattr;
	if (_mutatr_init(&mutextattr) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Init Mutex attr in DD task", _task_get_id());
	}

	if (_mutex_init(&active_task_mutex, &mutextattr) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Init active mutex", _task_get_id());
	}

	if (_mutex_init(&overdue_task_mutex, &mutextattr) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Init overdue mutex", _task_get_id());
	}

	_queue_id task_finished_queue = _msgq_open(FINISHED_QUEUE_ID, 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] failed to create Scheduler Finished Queue. Error: 0x%x",  _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	// create msg pool for task finished
	dd_message_pool = _msgpool_create(sizeof(TASK_FINISHED), FINISHED_MSG_POOL_SIZE, 0, 0);
	if (_task_get_error() != MQX_EOK) {
		printf("\r\n[%d] failed to Open Message Pool for task finished. nError 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	printf("\r\nStarting Scheduler");

#ifdef PEX_USE_RTOS
  	while (1) {
#endif


		//Set the priority of the leading task
		int mut_error = _mutex_lock(&active_task_mutex);
		if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Couldn't Lock active task mutex when adding to the list. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
		}
		TASK_LIST_PTR task_list_head_temp = task_list_head;

		while(task_list_head_temp != NULL ){
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

		mut_error = _mutex_unlock(&active_task_mutex);
		if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
			printf("\r\n[%d] Couldn't unlock active task mutex when adding to the list. Error: 0x%x", _task_get_id(), _task_get_error());
			_task_set_error(MQX_OK);
		}

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
	uint32_t timerStep = 2000;

	TIME_STRUCT elapsed_time;
	uint32_t previous_time = 0;

#ifdef PEX_USE_RTOS
  while (1) {
#endif

	 timer++;

	 if (timer == timerStep){
		 _time_get_elapsed(&elapsed_time);

		 uint32_t total_time = ((elapsed_time.SECONDS*1000 + elapsed_time.MILLISECONDS));
		 printf("\r\nTimer: %u", timer);
		 printf("\r\nTotal Time: %u", total_time);
		 uint32_t utilization = 100 * (5*timer) / total_time;
		 printf("\r\n Utilization: %u PERCENT", 100-utilization);
		 //printf("\r\n CPU Utilization = %u", utilization);
		 timerStep += timerStep;
		// previous_time = total_time;
		 utilization = 0;
	 }

	 _time_delay_ticks(1);
    
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

	printf("\r\nStarting Periodic Task");
	TASK_LIST_PTR active_list = NULL;
	TASK_LIST_PTR active_iter = NULL;
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */

	dd_tcreate(PERIODICTASK_TASK, 150);
	dd_tcreate(PERIODICTASK_TASK, 200);
	dd_tcreate(PERIODICTASK_TASK, 1000);
	dd_tcreate(PERIODICTASK_TASK, 150);
	dd_tcreate(PERIODICTASK_TASK, 200);
	dd_tcreate(PERIODICTASK_TASK, 1500);
	dd_tcreate(PERIODICTASK_TASK, 200);
	dd_tcreate(PERIODICTASK_TASK, 1000);
	dd_tcreate(PERIODICTASK_TASK, 150);
	dd_tcreate(PERIODICTASK_TASK, 2000);

	OSA_TimeDelay(1000);

    
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

	printf("\r\n[%d] Starting New Task: absdeadline == %u", _task_get_id(), task_init_data);
	TIME_STRUCT start_time;
	_time_get(&start_time);

	int ticker = 0;
	TIME_STRUCT current_time;
	do {
		_time_get(&current_time);
		_time_delay_ticks(1);
	} while ((current_time.SECONDS*1000) + current_time.MILLISECONDS < task_init_data);


	printf("\r\n Just Finished %d", _task_get_id());
	//printf("\r\n Loops %d took %d ms", loops, diff_time);

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
