/****************************
* developer: Neviu Mamu
* reviewer: Yossi Matzliah
* date: 01.03.2023
*****************************/

#include <stdio.h>	/* perror */
#include <stdlib.h>	/* malloc, free */
#include <assert.h>	/* asserts */

#include "uid.h"	/* UID libary */
#include "task.h"	/* my libary */
#include "pq.h"		/* prioraty queue libary */

/********** MACROS *******/

#define ERROR_TASK (1)
#define ERROR_QUEUE (2)
#define SUCCESS (0)

/************* managerial struct ********************/

struct task
{
   time_t exec_time;
   time_t interval;
   ilrd_uid_t uid; 
   int (*op_func)(void *);
   void *params;
};

/********** static ********/

static int IsSameTasks(const void *data, const void *param);

/******************* task functions ********************/

task_t *CreateTask(int (*op_func)(void *), void *params, time_t exec_time , time_t interval)
{
	task_t *new_task = NULL;
	
	assert(NULL != op_func);
	
	new_task = (task_t *) malloc(sizeof(task_t));
	
	if(NULL != new_task)
	{
		new_task->exec_time = exec_time;
		new_task->interval = interval;
		new_task->op_func = op_func;
		new_task->params = params;
	}
	
	return new_task;
}

int ReAddTask(pq_t * tasks,task_t *task)
{
	assert(NULL != tasks);
	assert(NULL != task);
	
	PQDequeue(tasks);
	
	return PQEnqueue(tasks, task);		
}

int RunTask(task_t *task)
{
	assert(NULL != task);
	
	return task->op_func(task->params);
}

task_t *GetFrontTask(pq_t *tasks)
{
	task_t *task = NULL;
	
	assert(NULL != tasks);
	
	task = (task_t *)PQPeek(tasks);
	
	return task;
}

void RemoveFrontTask(pq_t *tasks)
{
	task_t *reserve_to_free = NULL;
	
	assert(NULL != tasks);
	
	reserve_to_free = (task_t *)PQPeek(tasks);
	PQDequeue(tasks);
	free(reserve_to_free);
}

void EraseTask(pq_t *tasks, ilrd_uid_t uid)
{
	task_t *task = NULL;
	
	assert(NULL != tasks);
	 
	task = (task_t *)PQErase(tasks, IsSameTasks, &uid);
	
	if(NULL != task)
	{
		free(task);	
	}
}

void  DestroyAllTasks(pq_t *tasks)
{
	assert(NULL != tasks);
	
	while(!PQIsEmpty(tasks))
	{
		RemoveFrontTask(tasks);			
	}
}

task_t *SetTaskExecTime(task_t *task, time_t exec_time)
{
	assert(NULL != task);
	
	task->exec_time = exec_time;
	
	return task;
}

time_t GetTaskExecTime(task_t *task)
{
	assert(NULL != task);
	
	return task->exec_time;
}

time_t GetTaskIntervals(task_t *task)
{
	assert(NULL != task);
	
	return task->interval;
}

ilrd_uid_t GetTaskUID(task_t *task)
{
	assert(NULL != task);
	
	return task->uid;
}

void SetTaskUID(task_t *task,ilrd_uid_t uid)
{
	assert(NULL != task);
	
	task->uid = uid;
}

/**************** static functions ******************/

static int IsSameTasks(const void *data, const void *param)
{
	ilrd_uid_t *uid = (ilrd_uid_t *)param;
	task_t *task = (task_t *)data;
	
	ilrd_uid_t uid_by_value1 = *uid;
	ilrd_uid_t uid_by_value2 = task->uid;
	
	return UIDIsSame(uid_by_value1, uid_by_value2);
} 




















