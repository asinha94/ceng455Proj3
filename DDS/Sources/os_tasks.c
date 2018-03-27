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

static void add_task_to_overdue_list(TASK_LIST_PTR task) {
	/*add task to overdue list. doesn't need to be sorted*/
	printf("\r\nI am Overdue");
}

_task_id add_task_to_list(_task_id tid, uint32_t deadline) {
	int mut_error = _mutex_lock(&active_task_mutex);
	if (mut_error != MQX_EOK || _task_get_error() != MQX_EOK) {
		printf("\r\n[%d] Couldn't Lock active task mutex when adding to the list. Error: 0x%x", _task_get_id(), _task_get_error());
		_task_set_error(MQX_OK);
		return 0;
	}

	TASK_LIST_PTR task_list_new = _mem_alloc(sizeof(TASK_LIST));
	task_list_new->tid = tid;
	task_list_new->absdeadline = deadline;
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
/*
	if (_mutex_init(&task_exec_mutex, &mutextattr) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Init overdue mutex", _task_get_id());
	}
*/

#ifdef PEX_USE_RTOS
  	while (1) {
#endif

  		/*
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
*/
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
	return;
	uint32_t timer = 0;
	uint32_t timerStep = 200000;

#ifdef PEX_USE_RTOS
  while (1) {
#endif
	 _time_delay_ticks(1);
	 timer++;

	 printf("\r\Monitor task running");

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

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */

	dd_tcreate(PERIODICTASK_TASK, 10);
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
	OSA_TimeDelay(task_init_data);
	dd_delete(_task_get_id());
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
