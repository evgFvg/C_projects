/************************************
* hash table implementation		    *
* Developer: 	Neviu Mamu  	    *
* Reviewer:  	Alex                *
* Date1	:	    13.04.2023		    *
************************************/
/**********************************************/

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert pointers */

#include "dynamic_vector.h" /* vector libray */
#include "heap.h"   /* heap library */

/*****************MACORS**********************/

#define INIT_VECTOR_SIZE (3)

#define SUCCESS (0)
#define FAILURE (1)

#define LEFT_CHILD_INDEX(i) (2 * i + 1)
#define RIGHT_CHILD_INDEX(i) (2 * i + 2)
#define PARENT(i) ((i - 1) / 2)

#define HAS_LEFT_CHILD(index)(LEFT_CHILD_INDEX(index) < VectorSize(heap->vec))
#define HAS_RIGHT_CHILD(index)(RIGHT_CHILD_INDEX(index) < VectorSize(heap->vec))

#define GET_VALUT_AT(index) (*(void **)VectorGetAccessToElement(heap->vec, index)) 

/************ managerial struct ***********************************/
struct heap
{
	vector_t *vec;
    int (*cmp_func)(const void *lhs, const void *rhs);
};
typedef struct CmpFuncWrapper
{
    void *data;
    int (*cmp_func)(const void *lhs, const void *rhs);
}wrapper_t;

/***********************Forward declaration***********************************/

static void HeapifyDown(heap_t *heap, size_t index);

static void HeapifyUp(heap_t *heap, size_t index);

static void Swap(vector_t *vec, size_t lhs, size_t rhs);

static size_t FindMaxForHeapfiyUp(heap_t *heap, size_t index);

static size_t FindMaxForHeapfiyDown(heap_t *heap, size_t index);

static int CmpFuncWaraper(const void *data, const void *param);

/**********************************************************************/

heap_t *HeapCreate(int (*cmp_func)(const void *, const void *))
{
    heap_t *new_heap = NULL;

    assert(NULL != cmp_func);

    new_heap = (heap_t *)malloc(sizeof(heap_t));

    if (NULL != new_heap)
    { 
        new_heap->cmp_func = cmp_func;
        new_heap->vec = VectorCreate(INIT_VECTOR_SIZE, sizeof(size_t));

        if (NULL == new_heap->vec)
        {
            free(new_heap);
            new_heap = NULL;
        }
    }

    return new_heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vec);

    VectorDestroy(heap->vec);
    free(heap);

    heap = NULL;
}

int HeapPush(heap_t *heap, void *data)
{
    int status = SUCCESS;

    assert(NULL != heap);
    assert(NULL != heap->vec);

    status = VectorPushBack(heap->vec, (void *)&data);
    HeapifyUp(heap, VectorSize(heap->vec) - 1);
    
    return status;
}

void HeapPop(heap_t *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vec);

    if (0 != VectorSize(heap->vec))
    {        
        Swap(heap->vec, 0, VectorSize(heap->vec) - 1);
        VectorPopBack(heap->vec);
        HeapifyDown(heap, 0);
    }
}

void *HeapRemoveByKey(heap_t *heap, void *data)
{
    wrapper_t cmp_wrap = {0};

    assert(NULL != heap);
    assert(NULL != heap->vec);

    cmp_wrap.data = data;
    cmp_wrap.cmp_func = heap->cmp_func; 

    return HeapRemove(heap, CmpFuncWaraper, &cmp_wrap);
}

void *HeapRemove(heap_t *heap, int (*is_match_func)(const void *data, const void *param), const void *param)
{
    int status = FAILURE;
    size_t i = 0;
    void *curr = NULL;

    assert(NULL != heap);
    assert(NULL != heap->vec);

    for (i = 0; i < VectorSize(heap->vec) && FAILURE == status; ++i)
    {
        curr = GET_VALUT_AT(i); 
 
        if (is_match_func(curr, param))
        {
            status = SUCCESS;
            Swap(heap->vec, i, VectorSize(heap->vec) - 1);
            VectorPopBack(heap->vec);
            HeapifyDown(heap, i);
            HeapifyUp(heap, i);
        }
    }

    return SUCCESS == status ? curr : NULL;
}

size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vec);

    return VectorSize(heap->vec);
}

void *HeapPeek(const heap_t *heap)
{   
    assert(NULL != heap);
    assert(NULL != heap->vec);

    return *(void **)VectorGetAccessToElement(heap->vec, 0);
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vec);
    
    return VectorIsEmpty(heap->vec);
}

/******************************static Heapify up/down*******************************************/

static void HeapifyUp(heap_t *heap, size_t index)
{
    size_t new_index = 0;

    if (0 != index)
    {
        new_index = FindMaxForHeapfiyUp(heap, index);

        if (new_index != PARENT(index))  
        {
            Swap(heap->vec, index, PARENT(index));
            HeapifyUp(heap,PARENT(index));
        }
    }
}

static void HeapifyDown(heap_t *heap, size_t index)
{
    size_t bigger_child_index = 0;

    if (HAS_LEFT_CHILD(index))
    {
        bigger_child_index = FindMaxForHeapfiyDown(heap, index);

        if (index != bigger_child_index)
        {
            Swap(heap->vec, bigger_child_index, index);
            HeapifyDown(heap, bigger_child_index);
        }
    }
}

/*************************statics*********************************************/

static void Swap(vector_t *vec, size_t lhs, size_t rhs)
{
    void *tmp = 0;
    void *lhs_data = VectorGetAccessToElement(vec, lhs);
    void *rhs_data = VectorGetAccessToElement(vec, rhs);

    tmp = *(void **)lhs_data;
    *(void **)lhs_data = *(void **)rhs_data;
    *(void **)rhs_data = tmp;
}

static size_t FindMaxForHeapfiyDown(heap_t *heap, size_t index)
{
    size_t return_index = 0;

    void *left_child = NULL;
    void *right_child = NULL;
    void *bigger_child = NULL;
    void *parent = NULL;

    return_index = LEFT_CHILD_INDEX(index);
    left_child = GET_VALUT_AT(LEFT_CHILD_INDEX(index));
    bigger_child = left_child;
    parent = GET_VALUT_AT(index);

    if (HAS_RIGHT_CHILD(index))
    {
        right_child = GET_VALUT_AT(RIGHT_CHILD_INDEX(index));

        if(0 < heap->cmp_func(right_child, left_child))
        {
            return_index = RIGHT_CHILD_INDEX(index);
            bigger_child = right_child;
        }
    }

    if (0 < heap->cmp_func(parent, bigger_child))
    {
        return_index = index;
    }

    return return_index;
}

static size_t FindMaxForHeapfiyUp(heap_t *heap, size_t index)
{
    void *child = NULL;
    void *parent = NULL;

    child = GET_VALUT_AT(index);
    parent = GET_VALUT_AT(PARENT(index));

    if(0 < heap->cmp_func(parent, child))
    {
        index = PARENT(index);
    }

    return index;
}

static int CmpFuncWaraper(const void *data, const void *param)
{
    wrapper_t *wrap = (wrapper_t *)param;

    return (wrap->cmp_func(data, wrap->data) == 0);
}