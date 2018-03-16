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

struct task_list{
	uint32_t tid;
	uint32_t deadline;
	uint32_t task_type;
	uint32_t creation_time;
	struct task_list *next_cell;
	struct task_list *prev_cell;
};

struct overdue_tasks{
	uint32_t tid;
	uint32_t deadline;
	uint32_t task_type;
	uint32_t creation_time;
	struct task_list *next_cell;
	struct task_list *prev_cell;
};

_pool_id dd_create_pool;
_queue_id dd_create_queue;


void task_dd(os_task_param_t task_init_data)
{

	//initialize message pool for DDCreate()

	dd_create_pool = _msgpool_create(sizeof(CREATE_REQUEST), CREATE_MSG_POOL_SIZE,0,0);
	if(_task_get_error() != MQX_EOK){
		printf("create message pool not created OR error. who knows");
	}

	//initialize queue for receiving from DDCreate()
	dd_create_queue = _msgq_open(CREATE_QUEUE_ID, 0);
	if(_task_get_error() != MQX_EOK){
		printf("dd_create_queue queue not created OR error, who knows farm");
	}





  
#ifdef PEX_USE_RTOS
  while (1) {
#endif

	 //check if the CreateDD queue is empty
	  int dd_create_queue_count = 0;
	  dd_create_queue_count = _msgq_get_count(dd_create_queue);

	  //if its queue has a message
	  if(dd_create_queue_count > 0){
		  //check current tasks in task_list

		  //resort tasks in task list

		  //add to task_list based on priority

	  }

    
    
    
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
