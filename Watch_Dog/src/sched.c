/****************************
* developer: Neviu Mamu
* reviewer: Yossi Matzliah
* date: 01.03.2023
*****************************/

#include <stdlib.h>	/* malloc, free */
#include <assert.h>	/* asserts */
#include <unistd.h>	/* sleep */

#include "uid.h"	/* UID library */
#include "task.h"	/* task library */
#include "pq.h"		/* prioraty queue library */
#include "scheduler.h"	/* scheduler library */

/************* MACROS ****************************************************************/

#define STOP (1)
#define RUN (0)

#define SUCCESS (0)
#define FAILURE (1)
#define PQFAIL (-1)

/************* managerial struct *****************************************************/

struct scheduler 
{
	pq_t *tasks;
	int stop_flag;
};

/************** sort function ********************************************************/

static int TimePrioraty(const void *lhs, const void *rhs)
{
	task_t *lhs_task = (task_t *)lhs; 
	task_t *rhs_task = (task_t *)rhs;
	
	return (int)difftime(GetTaskExecTime(rhs_task), GetTaskExecTime(lhs_task));
} 
/*************************************************************************************/

/************ scheduler functions ****************************************************/

sch_t *SchCreate(void)
{
	sch_t *new_sch = (sch_t *) malloc(sizeof(sch_t));
	
	if (NULL == new_sch)
	{
		return NULL;
	}
	
	new_sch->stop_flag = RUN;
	
	new_sch->tasks = PQCreate(TimePrioraty);
	
	if (NULL == new_sch->tasks)
	{
		free(new_sch);
		new_sch = NULL;
	}
	
	return new_sch;
}

void SchDestroy(sch_t *scheduler)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->tasks);
	
	SchClear(scheduler);
	
	PQDestroy(scheduler->tasks);
	
	free(scheduler);
}

ilrd_uid_t SchAddTask(sch_t *scheduler ,int (*op_func)(void *), void *params, time_t exec_time , time_t interval)
{
	task_t *task = NULL;
	ilrd_uid_t uid = {0};
	
	assert(NULL != scheduler);
	assert(NULL != op_func);
	assert(NULL != scheduler->tasks); 
	
	task =  CreateTask(op_func, params, exec_time, interval);
	uid = UIDCreate();
	
	if(NULL == task || UIDIsSame(uid, UIDGetBad()))
	{
		return UIDGetBad();
	}
	
	SetTaskUID(task, uid);
		
	if(SUCCESS != PQEnqueue(scheduler->tasks,task))
	{
		free(task);
		uid = UIDGetBad();
	}
	
	return uid;  
}

void SchRemoveTask(sch_t *scheduler ,ilrd_uid_t task_uid)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->tasks);
	
	EraseTask(scheduler->tasks, task_uid);
}

int SchIsEmpty(const sch_t *scheduler)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->tasks);
	
	return PQIsEmpty(scheduler->tasks);
}

size_t SchSize(const sch_t *scheduler)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->tasks);
	
	return PQSize(scheduler->tasks);	
}

int SchRun(sch_t *scheduler)
{
	int diff_time = 0;
	time_t curr_time = 0;
	task_t *runner = NULL;
	int status = SUCCESS;
	
	assert(NULL != scheduler);
	
	scheduler->stop_flag = RUN;
	
	while(!PQIsEmpty(scheduler->tasks) && SUCCESS == status && STOP != scheduler->stop_flag)
	{
		runner = GetFrontTask(scheduler->tasks);
		curr_time = time(NULL);
		
		diff_time = (int)difftime(GetTaskExecTime(runner), curr_time);
		
		if(diff_time > 0)
		{
			sleep((unsigned int)diff_time);
		}
		
		status = RunTask(runner);
		curr_time = time(NULL);
			
		if(GetTaskIntervals(runner) > 0 && status == SUCCESS)
		{
			SetTaskExecTime(runner, curr_time + GetTaskIntervals(runner));
			status = ReAddTask(scheduler->tasks,runner) == SUCCESS ? SUCCESS : PQFAIL;	
		}
		else
		{
			 RemoveFrontTask(scheduler->tasks);
		}
	}
	
	return status;	
}

void SchStop(sch_t *scheduler)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->tasks);
	
	scheduler->stop_flag = STOP;
}

void SchClear(sch_t *scheduler)
{
	assert(NULL != scheduler);
	assert(NULL != scheduler->tasks);
	
	DestroyAllTasks(scheduler->tasks);
}


