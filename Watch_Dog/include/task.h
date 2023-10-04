/****************************
* developer: Neviu Mamu
* reviewer: Yossi Matzliah
* date: 01.03.2023
*****************************/

#ifndef __ILRD_OL139_40_TASK_H__
#define __ILRD_OL139_40_TASK_H__

#include "pq.h"		/* prioraty queue libary */
#include "uid.h"	/* UID libary */
#include <time.h>	/* time_t */


/************* managerial struct typedef ********************/

typedef struct task task_t;

/*********************** functions description *****************/

/*
* AddTask Description:
*	Allocates  a new task node with all the requsted parameters. 
*
* @params:
*	op_func - the users function.
	param - users parameter to be executed with op_func.
	exec_time - time of execution
	interval - time to be exected again after the first execution.
			note - iterval 0, will execute the task ones.    	
* @returns:
*	success - task_t pointer.
	failure - NULL.
*
* @Complexity
*	O(n)
*/
task_t *CreateTask(int (*op_func)(void *), void *params, time_t exec_time , time_t interval); 


/*
* RemoveFrontTask Description:
*	Removes the highest prioraty task from the list.
*
* @params:
*	tasks - list of tasks.
*
* @returns:
*	void.
*
* @Complexity
*	O(1)
*/
void RemoveFrontTask(pq_t *tasks);

/*
* RunTask Description:
*	Execute a given task.
*
* @params:
*	task - the task to execute.
*
* @returns:
*	return number acording to the user function output.
*
* @Complexity
*	O(n)
*/
int RunTask(task_t *task);

/*
* GetFrontTask Description:
*	Returns the task with the higest prioraty.
*
* @params:
*	tasks - list of tasks.
*
* @returns:
*	The function return task pointer.
*
* @Complexity
*	O(1)
*/ 
task_t *GetFrontTask(pq_t *tasks);

/*
* EraseTask Description:
*	Locates a given task by UID and erase it.
*
* @params:
*	tasks - list of tasks.
*	uid - the UID of a given task to be located and erased.
* @returns:
*	none.
*
* @Complexity
*	O(n)
*/
void EraseTask(pq_t *tasks, ilrd_uid_t uid);

/*
* DestroyAllTasks Description:
*	Destroy all the tasks in the task list.
*
* @params:
*	tasks - list of tasks.
*
* @returns:
*	none.
*
* @Complexity
*	O(n)
*/
void  DestroyAllTasks(pq_t *tasks);

/*
*
* SetTaskExecTime Description:
*	Sets execution time of a task.
*
* @params:
*	task - the task.
	exec_time - the new time to be set.
*
* @returns:
*	task_t poiter with new execution time.
*
* @Complexity
*	O(1)
*/
task_t *SetTaskExecTime(task_t *task, time_t exec_time);

/*
*
* GetTaskExecTime Description:
*	Returns the execution time of the given task.
*
* @params:
*	task - the task.
*
* @returns:
*	The execution time as time_t type.
*
* @Complexity
*	O(1)
*/
time_t GetTaskExecTime(task_t *task);

/*
*
* GetTaskExecTime Description:
*	Adds again a given task to the existing task list.
*
* @params:
*	tasks - list of tasks.
*	task- the task to be added.
*
* @returns:
*	0 - success.
*	1 - failed to add the task.
*
* @Complexity
*	O(n)
*/
int ReAddTask(pq_t * tasks,task_t *task);

/*
*
* GetTaskIntervals Description:
*	Returns the  time intervals of the given  task.
*
* @params:
*	task- the task.
*
* @returns:
*	Returns the intervals time as time_t type.
*
* @Complexity
*	O(1)
*/
time_t GetTaskIntervals(task_t *task);

/*
*
* GetTaskUID Description:
*	Returns the  given task UID.
*
* @params:
*	task- the task.
*
* @returns:
*	Returns the UID time.
*
* @Complexity
*	O(1)
*/
ilrd_uid_t GetTaskUID(task_t *task);

/*
*
* SetTaskUID Description:
*	Set UID for a given task.
*
* @params:
*	task- the task.
*
* @returns:
*	none.
*
* @Complexity
*	O(1)
*/
void SetTaskUID(task_t *task,ilrd_uid_t uid);

#endif

