#include <stddef.h>    /* size_t */
#include <stdlib.h>    /*malloc*/
#include <time.h>      /*time_t*/
#include <stdio.h>    /*perror*/
#include <assert.h>
#include <unistd.h> /*sleep*/

#include "scheduler.h"
#include "task.h"
#include "pq.h"    

typedef int (*op_func_ptr)(void *p);

struct scheduler
{
    pq_t *list;
    int is_stopped;
};

static int Time_tCompare(const void *lhs, const void *rhs);
static int FindMatchAndDelTask(const void *lhs, const void *rhs);

sch_t *SchCreate(void)
{
    sch_t *new_sch = (sch_t *)malloc(sizeof(sch_t));
    if (NULL == new_sch)
    {
        perror("Memory allocation problem\n");
        return NULL;
    }

    new_sch->list = PQCreate(Time_tCompare);

    if (NULL == new_sch->list)
    {
        perror("Memory allocation problem\n");
        free(new_sch);
        return NULL;
    }
    new_sch->is_stopped = 0;

    return new_sch;
}


void SchDestroy(sch_t *scheduler)
{
    assert(NULL != scheduler);
    assert(NULL != scheduler->list);
    
    SchClear(scheduler);
    
    PQDestroy(scheduler->list);
    
    free(scheduler);
}

ilrd_uid_t SchAddTask(sch_t *scheduler, int (*op_func)(void *), void *params, time_t exec_time, time_t interval)
{
    task_t *new_task = NULL;

    assert(NULL != scheduler);
    assert(NULL != scheduler->list);
    assert(NULL != op_func);

    new_task = TaskCreate(op_func, params, exec_time, interval);

    if (NULL == new_task)
    {
        return UIDGetBad();
    }

    return 0 == PQEnqueue(scheduler->list, new_task) ? TaskGetUID(new_task) : UIDGetBad();
}

void SchRemoveTask(sch_t *scheduler, ilrd_uid_t task_uid)
{
    assert(NULL != scheduler);
    assert(NULL != scheduler->list);

    PQErase(scheduler->list, FindMatchAndDelTask, &task_uid);
}

int SchIsEmpty(const sch_t *scheduler)
{
    assert(NULL != scheduler);
    assert(NULL != scheduler->list);

    return PQIsEmpty(scheduler->list);
}

size_t SchSize(const sch_t *scheduler)
{
    assert(NULL != scheduler);
    assert(NULL != scheduler->list);

    return PQSize(scheduler->list);
}


void SchClear(sch_t *scheduler)
{
    assert(NULL != scheduler);
    assert(NULL != scheduler->list);
    
    while (!SchIsEmpty(scheduler))
    {
        TaskDestroy((task_t *)PQPeek(scheduler->list));
       
        PQDequeue(scheduler->list);
    }
}


int SchRun(sch_t *scheduler)
{
    task_t *task = NULL;
    time_t interval = 0;
    time_t exec_time = 0;
    time_t diff = 0;
    int func_status = 0;

    assert(NULL != scheduler);
    assert(NULL != scheduler->list);

    while (0 == SchIsEmpty(scheduler) && 0 == scheduler->is_stopped && 0 == func_status)
    {
        task = (task_t *)PQPeek(scheduler->list);
        interval = TaskGetInterval(task);
        exec_time = TaskGetExecTime(task);

        if (0 < (diff = difftime(exec_time, time(NULL))))
        {
            sleep(diff);
        }

        func_status = TaskRun(task);

        if( 0 != interval)
        {
            TaskSetExecTime(task, exec_time + interval);
            PQEnqueue(scheduler->list, task);
            PQDequeue(scheduler->list);
        }
        else
        {
            SchRemoveTask(scheduler, TaskGetUID(task));
        } 
    }
    
    scheduler->is_stopped = 0;

    return func_status;
}

void SchStop(sch_t *scheduler)
{
    assert(NULL != scheduler);

    scheduler->is_stopped = 1;
}


/*******************************Static functions******************************/


static int FindMatchAndDelTask(const void *lhs, const void *rhs)
{
    task_t *task = (task_t *)lhs;
    ilrd_uid_t task_uid = TaskGetUID(task);
    ilrd_uid_t uid_to_del = *(ilrd_uid_t *)rhs;
    int res = 0;

    if( 1 == UIDIsSame(task_uid, uid_to_del))
    {
        TaskDestroy(task);
        res =  1;
    }

    return res;
}

static int Time_tCompare(const void *lhs, const void *rhs)
{
    task_t *task1 = (task_t *)lhs;
    task_t *task2 = (task_t *)rhs;

    time_t t1 = TaskGetExecTime(task1);
    time_t t2 = TaskGetExecTime(task2);

    return (int)difftime(t1, t2);
}
