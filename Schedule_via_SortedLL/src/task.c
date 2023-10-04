/*
 * Developer Evgenii F.
 * Reviewd by Nitai I.
 * Date 02.03.2023
 * 
 */

#include <stdlib.h>    /*malloc*/
#include <stdio.h> /*perror*/
#include <assert.h>

#include "task.h"

struct task
{
    ilrd_uid_t uid;
    time_t exec_time;
    time_t interval;
    void *param;
    int (*op_func)(void *p);
};

task_t *TaskCreate(int (*op_func)(void *), void *param, time_t exec_time, time_t interval)
{
    task_t *new_task = NULL;
    ilrd_uid_t uid_tmp = {0};

    assert(NULL != op_func);

    new_task = (task_t *)malloc(sizeof(task_t));
    if (NULL == new_task)
    {
        perror("Memory allocation error\n");
        return NULL;
    }

    new_task->exec_time = exec_time;
    new_task->interval = interval;
    new_task->param = param;
    new_task->op_func = op_func;
    uid_tmp = UIDCreate();

    if (1 == UIDIsSame(uid_tmp, UIDGetBad()))
    {
        perror("Creatin UID problem\n");
        free(new_task);
        return NULL;
    }

    new_task->uid = uid_tmp;

    return new_task;
}

void TaskDestroy(task_t *task)
{
    assert(NULL != task);

    free(task);
}

ilrd_uid_t TaskGetUID(const task_t *task)
{
    assert(NULL != task);

    return task->uid;
}

time_t TaskGetInterval(const task_t *task)
{
    assert(NULL != task);

    return task->interval;
}

time_t TaskGetExecTime(const task_t *task)
{
    assert(NULL != task);

    return task->exec_time;
}

void *TaskGetParam(const task_t *task)
{
    assert(NULL != task);

    return task->param;
}

int (*TaskGetFunc(const task_t *task))(void *) 
{
    assert(NULL != task);

    return task->op_func;
}

void TaskSetFunction(task_t *task, int (*new_func)(void *), void *param)
{
    assert(NULL != task);
    assert(NULL != new_func);

    task->op_func = new_func;
    task->param = param;
}

void TaskSetParam(task_t *task, void *new_param)
{
    assert(NULL != task);

    task->param = new_param;
}

void TaskSetExecTime(task_t *task, time_t new_exec_time)
{
    assert(NULL != task);

    task->exec_time = new_exec_time;
}

void TaskSetInterval(task_t *task, time_t new_interval)
{
    assert(NULL != task);

    task->interval = new_interval;
}

int TaskRun(task_t *task)
{
    assert(NULL != task);

    return task->op_func(task->param);
}
