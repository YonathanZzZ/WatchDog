#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "task.h"

enum Status
{
	SUCCESS = 0,
	FAILURE = 1
};

task_t *TaskCreate(int (*op_func)(void *), void *param, 
				   size_t delay_in_sec, size_t interval_in_sec, 
				   void (*task_cleanup)(void *), void *cleanup_param)
{
	task_t *task = NULL;
	
	assert(op_func);
	
	task = (task_t*)malloc(sizeof(task_t));
	if(NULL == task)
	{
		return (NULL);
	}
	
	/* initialize task struct */
	
	task->task_id = UIDCreate();
	if(UIDIsSame(task->task_id, UIDBadUID))
	{
		free(task);
		
		return (NULL);
	}
	
	task->op_func = op_func;
	task->op_param = param;
	task->time_to_run = time(0) + delay_in_sec;
	task->interval_in_sec = interval_in_sec;
	task->task_cleanup = task_cleanup;
	task->cleanup_param = cleanup_param;
	
	return (task);
}

uid_t TaskGetUID(const task_t *task)
{
	assert(task);
	
	return (task->task_id);
}

int TaskIsMatch(const task_t *task1, uid_t uid)
{
	assert(task1);
	
	return (UIDIsSame(TaskGetUID(task1), uid));
}

time_t TaskGetTimeToRun(const task_t *task)
{
	assert(task);
	
	return (task->time_to_run);
}

int TaskIsBefore(const task_t *to_check, const task_t *check_against)
{
	assert(to_check);
	assert(check_against);
	
	return (TaskGetTimeToRun(to_check) < TaskGetTimeToRun(check_against));
}

void TaskDestroy(task_t *task)
{
	assert(task);
	
	free (task);
}

int TaskUpdateTimeToRun(task_t *task)
{
	assert(task);
	/* the updated time to run is used to insert the task that's being executed 
	   back into the pqueue with an updated priority (which is based on 
	   time_to_run) */
	task->time_to_run += task->interval_in_sec;
		
	return (SUCCESS);
}

int TaskRun(task_t *task)
{	
	int return_status = DO_NOT_REPEAT;
	
	assert(task);

	return_status = task->op_func(task->op_param);
	
	if(NULL != task->task_cleanup)
		{
			task->task_cleanup(task->cleanup_param);
		}
		
	return (return_status);
}

