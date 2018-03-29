/* ###################################################################
**     Filename    : os_tasks.h
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
** @file os_tasks.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         

#ifndef __os_tasks_H
#define __os_tasks_H
/* MODULE os_tasks */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "mqx_ksdk.h"
#include "uart1.h"
#include "fsl_mpu1.h"
#include "fsl_hwtimer1.h"
#include "MainTask.h"
#include "DD.h"
#include "MonitorTask.h"
#include "ptask.h"
#include "PeriodicTask.h"
#include <message.h>

#ifdef __cplusplus
extern "C" {
#endif 

//Message Pool Sizes
#define FINISHED_MSG_POOL_SIZE 32

// Message Pool IDs
 _pool_id dd_message_pool;

//QUEUE IDs
#define FINISHED_QUEUE_ID 		5


#define HIGHEST_PRIORITY 8

typedef struct task_list{
	uint32_t tid;
	uint32_t task_type;
	uint32_t reldeadline;
	uint32_t absdeadline;
	struct task_list *next;
	//struct task_list *prev;
} TASK_LIST, * TASK_LIST_PTR;

typedef struct task_finsihed {
	MESSAGE_HEADER_STRUCT 	HEADER;
	_task_id 				tid;
	uint32_t 				finishtime;
} TASK_FINISHED, * TASK_FINISHED_PTR;

extern _task_id add_task_to_list(_task_id, uint32_t, uint32_t);
extern bool remove_from_task_list(_task_id, uint32_t);
extern void get_active_head(TASK_FINISHED_PTR *);
extern void get_overdue_head(TASK_FINISHED_PTR *);

/*
** ===================================================================
**     Callback    : task_dd
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void task_dd(os_task_param_t task_init_data);


/*
** ===================================================================
**     Callback    : monitor_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void monitor_task(os_task_param_t task_init_data);

/*
** ===================================================================
**     Callback    : p_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void p_task(os_task_param_t task_init_data);

/*
** ===================================================================
**     Callback    : Periodic_Task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Periodic_Task(os_task_param_t task_init_data);

/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __os_tasks_H*/
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
