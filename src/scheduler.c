#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcpy */
#include <unistd.h> /* sleep */

#include <scheduler.h>

struct Scheduler
{
	pq_t *pq;
	task_t *current_task;
	int is_stopped;
	int to_remove;
};

size_t SchedulerSize(const scheduler_t *scheduler)
{
	assert(scheduler);
	
	if(NULL != scheduler->current_task)
	{
		return (PQSize(scheduler->pq) + 1);
	}
	
	return (PQSize(scheduler->pq));
}

int SchedulerIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler);
	
	if(NULL == scheduler->current_task && PQIsEmpty(scheduler->pq))
	{
		return (TRUE);
	}
	
	return (FALSE);
}

static int CompareTime(const void *task1, const void *task2)
{
	return (((task_t*)task2)->time_to_run - ((task_t*)task1)->time_to_run);
}

scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if(NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->pq = PQCreate(CompareTime);
	if(NULL == scheduler->pq)
	{
		free(scheduler);
		
		return (NULL);
	}
	
	scheduler->is_stopped = 0;
	scheduler->to_remove = 0;
	scheduler->current_task = NULL;
	
	return (scheduler);
}

void SchedulerClear(scheduler_t *scheduler)
{
	task_t *task_to_remove = NULL;
	
	assert(scheduler);
	
	while (!SchedulerIsEmpty(scheduler))
	{
		task_to_remove = PQDequeue(scheduler->pq);
		TaskDestroy(task_to_remove);
	}
}

void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(scheduler);
	
	SchedulerClear(scheduler);
	
	PQDestroy(scheduler->pq);
	free(scheduler);
}

static int IsMatch(const void *task1, const void *uid)
{	
	uid_t uid1 = TaskGetUID((task_t*)task1);
	uid_t uid2 = *(uid_t*)uid;
	 
	return (UIDIsSame(uid1, uid2)); 
}

int SchedulerRemoveTask(scheduler_t *scheduler, uid_t uid)
{
	void *item_to_erase = NULL;
	
	assert(scheduler);
	
	/* check if task tries to destroy itself */
	if(NULL != scheduler->current_task && IsMatch(scheduler->current_task, &uid))
	{
		scheduler->to_remove = 1;
		
		return (SUCCESS);
	}
	
	/* find task in queue */
	item_to_erase = PQErase(scheduler->pq, IsMatch, &uid);
	if(NULL == item_to_erase)
	{
		return (ERROR);
	}
	
	TaskDestroy(item_to_erase); 
	
	return (SUCCESS); 

uid_t SchedulerAddTask(scheduler_t *scheduler, int (*op_func)(void *), 
					   		void *op_param, size_t delay_in_sec, 
					 		size_t interval_in_sec, void (*task_cleanup)(void *), 
					  		void *cleanup_param)
					  		
{
	task_t *task = NULL;
	
	assert(scheduler);
	assert(op_func);
	
	/* create task and add it to queue */
	task = TaskCreate(op_func, op_param, delay_in_sec, interval_in_sec, task_cleanup, 
		              cleanup_param);
	if(NULL == task)
	{
		return (UIDBadUID);
	}	
	
	if(0 != PQEnqueue(scheduler->pq, task))
	{
		TaskDestroy(task);
		
		return (UIDBadUID);
	}	
	
	return (TaskGetUID(task));   
}

void SchedulerStop(scheduler_t *scheduler)
{
	assert(scheduler);
	
	scheduler->is_stopped = 1;
}

int SchedulerRun(scheduler_t *scheduler)
{
	int task_status = 0;
	int enqueue_status = 0;
		
	assert(scheduler);
	
	if(SchedulerIsEmpty(scheduler))
	{
		return (SUCCESS);
	}
	
	while(!scheduler->is_stopped)
	{
		scheduler->to_remove = 0;
		
		scheduler->current_task = PQDequeue(scheduler->pq);
		
		if(time(0) < TaskGetTimeToRun(scheduler->current_task))
		{
			while(0 != sleep(TaskGetTimeToRun(scheduler->current_task) - time(0)));
		}
		
		task_status = TaskRun(scheduler->current_task);
		if(DO_NOT_REPEAT == task_status || scheduler->to_remove)
		{
			TaskDestroy(scheduler->current_task);
		}
		else
		{
			TaskUpdateTimeToRun(scheduler->current_task);
			
			enqueue_status = PQEnqueue(scheduler->pq, scheduler->current_task);
			if(SUCCESS != enqueue_status)
			{
				return (ERROR);
			}
		}
		
		scheduler->current_task = NULL;
		
		if(SchedulerIsEmpty(scheduler))
		{
			/* no tasks to run */
			return (SUCCESS);
		}
	}
	
	return (STOPPED);
}


