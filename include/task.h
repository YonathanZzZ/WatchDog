#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h> /* size_t */
#include <time.h> /* time_t */

#include "uid.h" 

enum TASK_RETURN_STATUS
{
	REPEAT = 0,
	DO_NOT_REPEAT = 1
};

typedef struct Task 
{
	uid_t task_id;
	int (*op_func)(void *); 
	void *op_param;
	time_t time_to_run;
	size_t interval_in_sec;
	void (*task_cleanup)(void *);
	void *cleanup_param;
}task_t;

/*****************************************************************************/
/*
Description: Create a task.
Arguments: 
	*op_func 		- valid operation function pointer
	*param 			- valid pointer to operation function parameter 
	delay_in_sec	- in how much time should task be executed
	interval_in_sec - time interval between task executions
	*task_cleanup  - valid pointer to task clean up function
	*cleanup_param - valid pointer to cleanup function parameter
Return: Pointer to created task.
Time complexity: O(1).
Space complexity: O(1).
*/

task_t *TaskCreate(int (*op_func)(void *), void *param, 
				   size_t delay_in_sec, size_t interval_in_sec, 
				   void (*task_cleanup)(void *), void *cleanup_param);
				   
/*****************************************************************************/
/*
Description: Destroy a task.
Arguments: 
	*task - valid task pointer
Return: Void.
Time complexity: O(1).
Space complexity: O(1).
*/
				
void TaskDestroy(task_t *task);

/*****************************************************************************/
/*
Description: Get task UID.
Arguments: 
	*task - valid task pointer
Return: UID of the task.
Time complexity: O(1).
Space complexity: O(1).
*/		
	
uid_t TaskGetUID(const task_t *task); 

/*****************************************************************************/
/*
Description: Get the time when the task will run.
Arguments:
	*task - valid task pointer
Return: The time when task will run.
Time complexity: O(1).
Space complexity: O(1).
*/			

time_t TaskGetTimeToRun(const task_t *task);

/*****************************************************************************/
/*
Description: Update the "time to run" of the task.
Arguments: 
	*task - valid task pointer
Return: SUCCESS (0) if time updated successfully, FAIL (-1) if not.
Time complexity: O(1).
Space complexity: O(1).
*/		
	
int TaskUpdateTimeToRun(task_t *task);

/*****************************************************************************/
/*
Description: Run the task pointed to.
Arguments:
	*task - valid task pointer
Return: (0) on success, (not 0) on fail.
Time complexity: O(1).
Space complexity: O(1).
*/			

int TaskRun(task_t *task); 

/*****************************************************************************/
/*
Description: Check if one task is before the other.
Arguments: 
	*to_check 	   - valid task pointer to the task to be checked
	*check_against - valid task pointer to the reference task
Return: TRUE (1) if to_check is bedore check_against, FALSE (0) if not.
Time complexity: O(1).
Space complexity: O(1).
*/			

int TaskIsBefore(const task_t *to_check, const task_t *check_against); 

/*****************************************************************************/
/*
Description: Check if a certain task matched a UID.
Arguments: 
	*task - valid task pointer to the task to be checked
	uid   - UID to be checked
Return: TRUE (1) if matching, FALSE (0) if not.
Time complexity: O(1).
Space complexity: O(1).
*/	
		
int TaskIsMatch(const task_t *task1, uid_t uid); 


#endif /* __TASK_H__ */
