/*************************************************
* developer: Evgenii
* reviewer: Arik
* date: 28.02.2023
*
**************************************************/

#include<assert.h>
#include <stdio.h>  /* perror*/
#include <stdlib.h>  /* malloc*/

#include "pq.h"
#include "sorted_list.h"

struct pq
{
    sol_t *list;
};

/**********************************Functions********************************/

pq_t *PQCreate(int (*priority_func)(const void *lhs, const void *rhs))
{
    pq_t *pq = (pq_t *)malloc(sizeof(pq_t));
    
    assert(NULL != priority_func);
    
    if (NULL == pq)
    {
        perror("Memory allocation problem\n");
        return NULL;
    }
    
    pq->list = SortedListCreate(priority_func);
    
    if (NULL == pq->list)
    {
        perror("Memory allocation problem\n");
        free(pq);
        return NULL;
    }
    
    return pq;
}

void PQDestroy(pq_t *pq)
{
    assert(NULL != pq);
    
    SortedListDestroy(pq->list);
    free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
    iterator_st iter = {0};

    assert(NULL != pq);

    iter = SortedListInsert(pq->list, data);

    return SortedListIsSameIter(iter, SortedListEndIter(pq->list));
}

void PQDequeue(pq_t *pq)
{
    assert(NULL != pq);
    assert(!PQIsEmpty(pq));

    SortedListPopFront(pq->list);
}

void *PQErase(pq_t *pq, int (*is_match)(const void *data, const void *param), void *param)
{
    void *ret_val = NULL;
    iterator_st start = {0};
    iterator_st end = {0};
    iterator_st match_iter = {0};

    assert(NULL != pq);
    assert(NULL != is_match);

    start = SortedListBeginIter(pq->list);
    end = SortedListEndIter(pq->list);
    match_iter = SortedListFindIf(start, end, is_match, param);

    if (!SortedListIsSameIter(end, match_iter))
    {
        ret_val = SortedListGetData(match_iter);
        SortedListRemove(match_iter);
    }

    return ret_val;
}

void PQClear(pq_t *pq)
{
    assert(NULL != pq);

    while(!PQIsEmpty(pq))
    {
        PQDequeue(pq);
    }
}

int PQIsEmpty(const pq_t *pq)
{
    assert(NULL != pq);

    return SortedListIsEmpty(pq->list);
}

void *PQPeek(const pq_t *pq)
{
    assert(NULL != pq);
    assert(!PQIsEmpty(pq));

    return SortedListGetData(SortedListBeginIter(pq->list));
}

size_t PQSize(const pq_t *pq)
{
    assert(NULL != pq);

    return SortedListSize(pq->list);
}



pq_t *PQMerge(pq_t *pq_dest, pq_t *pq_src)
{
    assert(NULL != pq_dest);
    assert(NULL != pq_src);

    pq_dest->list = SortedListMerge(pq_dest->list, pq_src->list);

    return pq_dest;
}
