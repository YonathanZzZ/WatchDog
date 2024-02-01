#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "pqueue.h" 
#include "task.h"

typedef struct Scheduler scheduler_t;

enum
{
	FALSE = 0,
	TRUE = 1
};

enum SCHEDULER_RUN_STATUS
{
	SUCCESS = 0,
	ERROR = 1,
	STOPPED = 2 
};

/*****************************************************************************/
/*
Description: Create a scheduler.
Arguments: Void.
Return: A pointer to the created scheduler.
Time complexity: O(1).
Space complexity: O(1).
*/

scheduler_t *SchedulerCreate(void);

/*****************************************************************************/
/*
Description: Destroy a scheduler.
Arguments: 
	*scheduler - valid scheduler pointer
Return: Void.
Time complexity: O(n).
Space complexity: O(1).
*/

void SchedulerDestroy(scheduler_t *scheduler);

/*****************************************************************************/
/*
Description: Add task to the scheduler. 
Arguments: 
	*scheduler 		- valid scheduler pointer
	*op_func 		- valid operation function pointer
	*param 			- valid pointer to operation function parameter 
	delay_in_sec	- in how much time should task be executed
	interval_in_sec - time interval between task executions
	*task_cleanup   - valid pointer to task clean up function
	*cleanup_param  - valid pointer to cleanup function parameter
Return: UID of created task. return BadUID on failure.
Time complexity: O(n).
Space complexity: O(1).
*/

uid_t SchedulerAddTask(scheduler_t *scheduler, int (*op_func)(void *), 
					   		void *op_param, size_t delay_in_sec, 
					 		size_t interval_in_sec, void (*task_cleanup)(void *), 
					  		void *cleanup_param);
					   
/*****************************************************************************/
/*
Description: Remove a task.
Arguments: 
	*scheduler - valid scheduler pointer
	uid   - UID of task to be removed
Return: SUCCESS / ERROR.
Time complexity: O(n).
Space complexity: O(1).
*/

int SchedulerRemoveTask(scheduler_t *scheduler, uid_t uid);

/*****************************************************************************/
/*
Description: Run the scheduler.
Arguments: 
	*scheduler - valid scheduler pointer
Return: ERROR when function fails, STOPPED if proccess stopped and SUCCESS on
		successful running all tasks.
Time complexity: O(n).
Space complexity: O(1).
*/

int SchedulerRun(scheduler_t *scheduler);

/*****************************************************************************/
/*
Description: Stop the scheduler.
Arguments: 
	*scheduler - valid scheduler pointer
Return: Void.
Time complexity: O(n).
Space complexity: O(1).
*/

void SchedulerStop(scheduler_t *scheduler);

/*****************************************************************************/
/*
Description: Get the size of a scheduler.
Arguments: 
	*scheduler - valid scheduler pointer
Return: Number of tasks currently in scheduler.
Time complexity: O(n).
Space complexity: O(1).
*/

size_t SchedulerSize(const scheduler_t *scheduler);

/*****************************************************************************/
/*
Description: Check if scheduler is empty.
Arguments: 
	*scheduler - valid scheduler pointer
Return: TRUE (1) / FALSE (0).
Time complexity: O(1).
Space complexity: O(1).
*/

int SchedulerIsEmpty(const scheduler_t *scheduler);

/*****************************************************************************/
/*
Description: Clear the scheduler of all tasks.
Arguments: 
	*scheduler - valid scheduler pointer
Return: Void.
Time complexity: O(n).
Space complexity: O(1).
*/

void SchedulerClear(scheduler_t *scheduler);

/*****************************************************************************/

#endif /* __SCHEDULER_H__ */


