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
#include <message.h>

#ifdef __cplusplus
extern "C" {
#endif 

//Message Pool Sizes
#define CREATE_MSG_POOL_SIZE 32
#define DELETE_MSG_POOL_SIZE 32
#define RETURN_MSG_POOL_SIZE 32

// Message Pool IDs
extern _pool_id dd_create_pool;
extern _pool_id dd_delete_pool;
extern _pool_id dd_return_pool;

//QUEUE IDs
#define CREATE_QUEUE_ID 		1
#define DELETE_QUEUE_ID 		2
#define RETURN_ACTIVE_QUEUE_ID 	3
#define RETURN_OVERDUE_QUEUE_ID 4

typedef struct task_list{
	uint32_t tid;
	uint32_t task_type;
	TIME_STRUCT creation_time;
	uint32_t deadline;
	struct task_list *next;
} TASK_LIST, * TASK_LIST_PTR;

typedef struct create_request {
	MESSAGE_HEADER_STRUCT HEADER;
	_task_id tid;
	uint32_t task_type;
	TIME_STRUCT creation_time;
	uint32_t deadline;
}	CREATE_REQUEST, * CREATE_REQUEST_PTR;

typedef struct delete_request {
	MESSAGE_HEADER_STRUCT HEADER;
	_task_id tid;
}	DELETE_REQUEST, * DELETE_REQUEST_PTR;

typedef struct return_request {
	MESSAGE_HEADER_STRUCT HEADER;
	TASK_LIST_PTR list;
}	RETURN_REQUEST, * RETURN_REQUEST_PTR;


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
