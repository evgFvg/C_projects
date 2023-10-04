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
#include "heap.h"


struct pq
{
    heap_t *heap;
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
    
    pq->heap = HeapCreate(priority_func);
    
    if (NULL == pq->heap)
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
    
    HeapDestroy(pq->heap);
    free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
    assert(NULL != pq);

    return HeapPush(pq->heap, data);
}

void PQDequeue(pq_t *pq)
{
    assert(NULL != pq);
    assert(!PQIsEmpty(pq));

    HeapPop(pq->heap);
}

void *PQErase(pq_t *pq, int (*is_match)(const void *data, const void *param), void *param)
{
    assert(NULL != pq);
    assert(NULL != is_match);

    return  HeapRemove(pq->heap, is_match, param);
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

    return HeapIsEmpty(pq->heap);
}

void *PQPeek(const pq_t *pq)
{
    assert(NULL != pq);
    assert(!PQIsEmpty(pq));

    return HeapPeek(pq->heap);
}

size_t PQSize(const pq_t *pq)
{
    assert(NULL != pq);

    return HeapSize(pq->heap);
}

