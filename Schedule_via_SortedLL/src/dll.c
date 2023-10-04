/********************************************
*	Developer:	Evgenii F.
*	Reviewer:       Aaron S.					
*	Date:		18/02/2023												
********************************************/

#include <assert.h> 
#include <stdlib.h> /* malloc*/
#include <stdio.h> /* perror*/
#include "dll.h"

static node_t *IterToNode(iterator_t iter);
static iterator_t NodeToIter(node_t *node);
static void ConnectNodes(node_t *a , node_t *b);
static iterator_t GetTail(iterator_t iter);
static int CountNodes(void *list_data, void *count);

struct node
{
    void *data;
    node_t *next;
    node_t *prev;
};

struct doubly_linked_list
{
    node_t *head;
    node_t *tail;
};


/**********************Functions***********************************/

dll_t *DLLCreate()
{
    dll_t *new_dll = (dll_t *)malloc(sizeof(dll_t));
    node_t *head_dummy = NULL;
    node_t *tail_dummy = NULL;

    if (NULL == (new_dll))
    {
        perror("Memory allocation error\n");
        return NULL;
    }

    head_dummy = (node_t *)malloc(sizeof(node_t));

    if (NULL == (head_dummy))
    {
        perror("Memory allocation error\n");
        free(new_dll);
        return NULL;
    }

    tail_dummy = (node_t *)malloc(sizeof(node_t));

    if (NULL == (tail_dummy))
    {
        perror("Memory allocation error\n");
        free(new_dll);
        free(head_dummy);
        return NULL;
    }

    tail_dummy->next = NULL;
    head_dummy->prev = NULL;

    tail_dummy->prev = head_dummy;
    head_dummy->next = tail_dummy;

    new_dll->head = head_dummy;
    new_dll->tail = tail_dummy;

    return new_dll;
}

void DLLDestroy(dll_t *dll)
{
    node_t *tmp = NULL;
    node_t *runner = NULL;

    assert(NULL != dll);

    runner = IterToNode(DLLEndIter(dll));

    while (NULL != runner)
    {
        tmp = runner;
        runner = runner->prev;
        free(tmp);
    }

    free(dll);
}

iterator_t DLLBeginIter(const dll_t *dll)
{
    assert(NULL != dll);

    return NodeToIter(dll->head->next);
}

iterator_t DLLEndIter(const dll_t *dll)
{
    assert(NULL != dll);

    return NodeToIter(dll->tail);
}

iterator_t DLLNextIter(const iterator_t iterator)
{
    node_t *res = NULL;

    assert(NULL != iterator);

    res = IterToNode(iterator);

    return NodeToIter(res->next);
}

iterator_t DLLPrevIter(const iterator_t iterator)
{
    node_t *res = NULL;

    assert(NULL != iterator);

    res = IterToNode(iterator);

    return NodeToIter(res->prev);
}

int DLLIsSameIter(const iterator_t iter1, const iterator_t iter2)
{
    assert(NULL != iter1);
    assert(NULL != iter2);

    return iter1 == iter2;
}

int DLLIsEmpty(const dll_t *dll)
{
    assert(NULL != dll);

    return (dll->head->next == dll->tail);
}

void DLLSetData(const iterator_t iterator, void *data)
{
    node_t *tmp = NULL;

    assert(NULL != iterator);

    tmp = NodeToIter(iterator);

    assert(NULL != tmp->next);
    assert(NULL != tmp->prev);

    tmp->data = data;
}

void *DLLGetData(iterator_t iterator)
{
    node_t *tmp = NULL;

    assert(NULL != iterator);

    tmp = NodeToIter(iterator);

    assert(NULL != tmp->next);
    assert(NULL != tmp->prev);

    return IterToNode(iterator)->data;
}

iterator_t DLLFind(iterator_t from, iterator_t to, is_match_t user_func, void *param)
{
    node_t *start = NULL;
    node_t *end = NULL;

    assert(NULL != from);
    assert(NULL != to);

    start = IterToNode(from);
    end = IterToNode(to);

    while (start != end && 0 == user_func(start->data, param))
    {
        start = start->next;
    }

    return NodeToIter(start);
}

int DLLForEach(iterator_t from, const iterator_t to, action_func_t user_func, void *param)
{
    node_t *start = NULL;
    node_t *end = NULL;
    int res = 0;

    assert(NULL != from);
    assert(NULL != to);

    start = IterToNode(from);
    end = IterToNode(to);

    while (start != end && 0 == res)
    {
        res = user_func(start->data, param);
        start = start->next;
    }

    return res;
}

size_t DLLCount(const dll_t *dll)
{
    size_t count = 0;
    DLLForEach(NodeToIter(dll->head->next), NodeToIter(dll->tail), CountNodes, (void *)&count);
    
    return count;
}

iterator_t DLLInsert(iterator_t iterator, void *data)
{
    node_t *iter_node = NULL;
    node_t *before_iter = NULL;
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    
    assert(NULL != iterator);
    iter_node = IterToNode(iterator);
    before_iter = iter_node->prev;

    if (NULL == (new_node))
    {
        perror("Memory allocation error\n");

        return GetTail(iterator);
    }
    else
    {
        new_node->data = data;
        new_node->next = iter_node;
        new_node->prev = iter_node->prev;

        iter_node->prev = new_node;
        before_iter->next = new_node;
    }

    return NodeToIter(new_node);
}

iterator_t DLLPushFront(dll_t *dll, void *data)
{

    iterator_t iter = NULL;

    assert(NULL != dll);

    iter = DLLBeginIter(dll);

    return DLLInsert(iter, data);
}

iterator_t DLLPushBack(dll_t *dll, void *data)
{
    iterator_t iter = NULL;

    assert(NULL != dll);

    iter = DLLEndIter(dll);

    return DLLInsert(iter, data);
}

iterator_t DLLRemove(iterator_t iterator)
{

    node_t *iter_node = NULL;
    node_t *nexToIter = NULL;
    node_t *beforeIter = NULL;

    assert(NULL != iterator);

    iter_node = IterToNode(iterator);
    nexToIter = iter_node->next;
    beforeIter = iter_node->prev;

    ConnectNodes(beforeIter, nexToIter);
    free(iter_node);

    return NodeToIter(nexToIter);
}

void *DLLPopFront(dll_t *dll)
{
    iterator_t first_elm = NULL;
    void *data = NULL;

    assert(NULL != dll);

    first_elm = DLLBeginIter(dll);
    data = first_elm->data;
    DLLRemove(first_elm);

    return data;
}

void *DLLPopBack(dll_t *dll)
{
    iterator_t last_elm = NULL;
    void *data = NULL;

    assert(NULL != dll);

    last_elm = DLLPrevIter(DLLEndIter(dll));
    data = last_elm->data;
    DLLRemove(last_elm);

    return data;
}

dll_t *DLLMultiFind(dll_t *dll_dest, iterator_t from, iterator_t to, is_match_t user_func, void *param)
{
    node_t *start = NULL;
    node_t *end = NULL;

    assert(NULL != dll_dest);
    assert(NULL != from);
    assert(NULL != to);

    start = IterToNode(from);
    end = IterToNode(to);

    while (start != end && 0 ==  user_func(start->data, param))
    {
        DLLPushFront(dll_dest, start->data);
        start = start->next;
    }

    return dll_dest;
}

iterator_t DLLSplice(iterator_t dest, iterator_t src_from, iterator_t src_to)
{
    node_t *from = NULL;
    node_t *before_from = NULL;

    node_t *to = NULL;
    node_t *before_to = NULL;

    node_t *dest_node = NULL;
    node_t *before_dest = NULL;

    assert(NULL != dest);
    assert(NULL != src_from);
    assert(NULL != src_to);

    dest_node = IterToNode(dest);
    before_dest = dest_node->prev;
    from = IterToNode(src_from);
    to = IterToNode(src_to);
    before_from = from->prev;
    before_to = to->prev;

    ConnectNodes(before_dest, from);
    ConnectNodes(before_to, dest_node);
    ConnectNodes(before_from, to);

    return src_from;
}

/*******************Static functions*******************************/

static node_t *IterToNode(iterator_t iter)
{
    return (node_t *)iter;
}

static iterator_t NodeToIter(node_t *node)
{
    return (iterator_t)node;
}

static void ConnectNodes(node_t *a , node_t *b)
{
    a->next = b;
    b->prev = a;
}

static iterator_t GetTail(iterator_t iter)
{
    node_t *runner = IterToNode(iter);

    while (NULL != runner)
    {
        runner = runner->next;
    }

    return NodeToIter(runner);
}

static int CountNodes(void *list_data, void *count)
{
    (void)list_data;
    ++(*(size_t *)count);
    return 0;
}
