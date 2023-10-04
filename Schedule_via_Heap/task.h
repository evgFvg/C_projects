#ifndef __Task_H__
#define __Task_H__


#include "uid.h"

typedef struct task task_t;
/*
* TaskCreate Description:
*	Allocates memory for a new task object.
*
* @params:
*   op_func   - Task function pointer.
*             - Return value of 0 is for success: any other return value will stop the execution of tasks
*   params    - Parameter used by task function.
*   exec_time - Time to execute the given task.
*   interval  - Interval of task execution, zero reserved for no repeats. 
*  		Any value that diffrent from 0 provides non stop execution in the given interval
*		
*
* @returns:
*	task_t * - a pointer to the new task object.
*   	Incase of failure return's NULL
* @Complexity
*	O(1)
*/
task_t *TaskCreate(int (*op_func)(void *), void *param, time_t exec_time , time_t interval);
/*
* TaskDestroy Description:
*	Releases the memory alloacted for a Task object.
*  	All refrences to the Task or related tasks become invalid.
*
* @params:
*   	task - pointer to a task_t object
*  	If the pointer is invalid, behavior is undefined.
*
* @returns:
*	void
*
* @Complexity
*	O(n)
*/
void TaskDestroy(task_t *task);

/*
* TaskGetUID Description:
*	Returns uid of the given task.
*
* @params:
*  	task - Pointer to task obj.
*   	If the pointer is invalid, behavior is undefined.
*
* @returns:
*	ilrd_uid_t - unique id of the task.
*
* @Complexity
*	O(1)
*/
ilrd_uid_t TaskGetUID(const task_t *task);
/*
* TaskGetFunc Description:
*	Returns pointer to function of the given task.
*
* @params:
*  	task - Pointer to task obj.
*   	If the pointer is invalid, behavior is undefined.
*
* @returns:
*	int (*op_func)(void *) - pointer to user's function
*
* @Complexity
*	O(1)
*/
int (*TaskGetFunc(const task_t *task))(void *) ;
/*
* TaskGetInterval Description:
*	Returns interval between executions of the given task.
*
* @params:
*  	task - Pointer to task obj.
*   	If the pointer is invalid, behavior is undefined.
*
* @returns:
*	time_t - interval time between executions.
*
* @Complexity
*	O(1)
*/
time_t TaskGetInterval(const task_t *task);
/*
* TaskGetParam Description:
*	Returns function param value of the given task.
*
* @params:
*  	task - Pointer to task obj.
*   	If the pointer is invalid, behavior is undefined.
*
* @returns:
*	void * - value to be used as an argument in the task function .
*
* @Complexity
*	O(1)
*/
void *TaskGetParam(const task_t *task);
/*
* TaskGetExecTime Description:
*	Returns execution time of the given task.
*
* @params:
*  	task - Pointer to task obj.
*   	If the pointer is invalid, behavior is undefined.
*
* @returns:
*	time_t - time execution time of the given task.
*
* @Complexity
*	O(1)
*/
time_t TaskGetExecTime(const task_t *task);

/*
* TaskSetFunction Description:
*	sets the new function in the task, with new param.
*
* @Params:
*	task - Pointer to task.
*  	If the pointer is invalid, behavior is undefined.
*	new_func - Pointer to the new function to be executed.
*  	If the pointer is invalid, behavior is undefined.
*	param - the data to use in the given function.
*	If the pointer is invalid, behavior is undefined.
*
* @Returns:
*	void
*
* @Complexity
*	Time: O(1)
*/
void TaskSetFunction(task_t *task, int (*new_func)(void *), void *param);


/*
* TaskSetExecTime Description:
*	sets the new execution time in the task.
*
* @Params:
*	task - Pointer to task.
*  	If the pointer is invalid, behavior is undefined.
*	new_exec_time - new execution time to be set
*
* @Returns:
*	void
*
* @Complexity
*	Time: O(1)
*/
void TaskSetExecTime(task_t *task, time_t new_exec_time);

/*
* TaskSetInterval Description:
*	sets the new interval time in the task.
*
* @Params:
*	task - Pointer to task.
*  	If the pointer is invalid, behavior is undefined.
*	interval - new interval time to be set
*
* @Returns:
*	void
*
* @Complexity
*	Time: O(1)
*/
void TaskSetInterval(task_t *task, time_t new_interval);

/*
* TaskSetParam Description:
*	sets the new function param in the task.
*
* @Params:
*	task - Pointer to task.
*  	If the pointer is invalid, behavior is undefined.
*	new_param - new param time to be set in the function
*
* @Returns:
*	void
*
* @Complexity
*	Time: O(1)
*/
void TaskSetParam(task_t *task, void *new_param);
/*
* TaskRun Description:
*	Executes the given task.
*
* @params:
*  	task - Pointer to task obj.
*   	If the pointer is invalid, behavior is undefined.
*
* @returns:
*	Return value of operation function assigned to task.
*  	0 - The tasks completed succesfully.
*   	any other value - function completed unseccessfully
*
* @Complexity
*	O(n)
*/

int TaskRun(task_t *task);

#endif

