#include <stdio.h>

#include <assert.h> /* assert*/
#include <stdlib.h> /* malloc, free */

#include "heap.h"
#include "vector.h"

#define EQUAL (0)
#define SUCCESS (0)
#define FAILURE (1)
#define TRUE (1)
#define INIT_CAPACITY (3)
#define ROOT_INDEX (0)
#define LEFT_CHILD_I(index) ((index)*2 + 1)
#define RIGHT_CHILD_I(index) ((index)*2 + 2)
#define PARENT_I(index) ((index - 1) / 2)

struct heap
{
    vector_t *vector;
    int (*cmp_func)(const void *, const void *);
};

static void *Remove(heap_t *heap, int (*locate_func)(const void *, const void *), const void *param, int pass_value);
static void HeapifyUp(heap_t *heap, size_t index);
static void HeapifyDown(heap_t *heap, size_t index);
static void Swap(void **a, void **b);
static void **GetData(heap_t *heap, size_t index);

/*** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***/

heap_t *HeapCreate(int (*cmp_func)(const void *, const void *))
{
    heap_t *heap = NULL;

    assert(NULL != cmp_func);

    heap = (heap_t *)malloc(sizeof(heap_t));

    if (NULL == heap)
    {
        return NULL;
    }

    heap->vector = VectorCreate(INIT_CAPACITY, sizeof(void *));

    if (NULL == heap->vector)
    {
        free(heap);
        return NULL;
    }

    heap->cmp_func = cmp_func;

    return heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);

    VectorDestroy(heap->vector);

    free(heap);
}

int HeapPush(heap_t *heap, void *data)
{
    int status = SUCCESS;

    assert(NULL != heap);
    assert(NULL != data);

    status = VectorPushBack(heap->vector, data);
    HeapifyUp(heap, VectorSize(heap->vector) - 1); /* index = size - 1 */

    return status;
}

void *HeapRemoveByKey(heap_t *heap, void *data)
{
    assert(NULL != heap);
    assert(NULL != data);

    return Remove(heap, heap->cmp_func, data, EQUAL);
}

static void *Remove(heap_t *heap, int (*locate_func)(const void *, const void *), const void *param, int pass_value)
{
    void **data = NULL;
    void *return_data = NULL;
    size_t index = 0;
    size_t size = 0;

    assert(NULL != heap);
    assert(NULL != locate_func);
    assert(0 != VectorSize(heap->vector));

    size = VectorSize(heap->vector);

    for (index = 0; index < size; ++index)
    {
        data = GetData(heap, index);

        if (pass_value == locate_func(*data, param))
        {
            Swap(GetData(heap, index), GetData(heap, size - 1));
            return_data = *GetData(heap, size - 1);
            VectorPopBack(heap->vector);
            HeapifyDown(heap, index);
            HeapifyUp(heap, index);
            break;
        }
    }

    return return_data;
}

void *HeapRemove(heap_t *heap, int (*is_match_func)(const void *data, const void *param), const void *param)
{
    assert(NULL != heap);
    assert(NULL != is_match_func);

    return Remove(heap, is_match_func, param, TRUE);
}

void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);

    return (*(GetData((void *)heap, 0)));
}

size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);

    return (VectorSize(heap->vector));
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);

    return (VectorIsEmpty(heap->vector));
}


void HeapPop(heap_t *heap)
{

    size_t index = 0;

    assert(NULL != heap);
    assert(0 != VectorSize(heap->vector));

    index = VectorSize(heap->vector) - 1;

    if (index > 0)
    {
        Swap(GetData(heap, ROOT_INDEX), GetData(heap, index));
    }

    VectorPopBack(heap->vector);
    HeapifyDown(heap, ROOT_INDEX);
}

static void HeapifyDown(heap_t *heap, size_t index)
{
    size_t top_index = index;
    size_t size = 0;

    assert(NULL != heap);

    size = VectorSize(heap->vector);

    if (LEFT_CHILD_I(top_index) < size &&
        heap->cmp_func(*GetData(heap, top_index), *GetData(heap, LEFT_CHILD_I(index))) < 0)
    {
        top_index = LEFT_CHILD_I(index);
    }

    if (RIGHT_CHILD_I(index) < size && heap->cmp_func(*GetData(heap, top_index), *GetData(heap, RIGHT_CHILD_I(index))) < 0)
    {
        top_index = RIGHT_CHILD_I(index);
    }

    if (top_index != index)
    {
        Swap(GetData(heap, top_index), GetData(heap, index));
        HeapifyDown(heap, top_index);
    }
}

static void HeapifyUp(heap_t *heap, size_t index)
{
    assert(NULL != heap);

    if (0 == index)
    {
        return;
    }

    if (0 < heap->cmp_func(*GetData(heap, index), *GetData(heap, PARENT_I(index))))
    {
        Swap(GetData(heap, index), GetData(heap, PARENT_I(index)));
        HeapifyUp(heap, PARENT_I(index));
    }
}

static void **GetData(heap_t *heap, size_t index)
{
    assert(NULL != heap);

    return VectorGetAccessToElement(heap->vector, index);
}

static void Swap(void **a, void **b)
{
    void *tmp = *a;

    assert(NULL != a || NULL != b);

    *a = *b;
    *b = tmp;
}
