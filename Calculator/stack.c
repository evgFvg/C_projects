/*************************************************
 * developer: Evgenii F.
 * reviewer: Neviu M.
 * date: 30.01.2023
 *
 **************************************************/
#include <stdlib.h> /*malloc*/
#include <stdio.h> /*perror*/
#include <string.h> /*memcpy*/
#include <assert.h>
#include "stack.h"

struct stack
{
    void *buffer;
    size_t top;
    size_t capacity;
    size_t element_size;
};

stack_t *StackCreate(size_t capacity, size_t element_size)
{
    stack_t *new_stack = (stack_t *)malloc(sizeof(stack_t));
    stack_t *res = new_stack;
    if (NULL == (new_stack))
    {
        perror("Memory allocation error\n");
        res = NULL;
    }
    else
    {
        new_stack->top = 0;
        new_stack->capacity = capacity;
        new_stack->element_size = element_size;
        new_stack->buffer = malloc(capacity * element_size);
        if (NULL == new_stack->buffer)
        {
            perror("Memory allocation error\n");
            res = NULL;
        }
    }
    return res;
}

void StackDestroy(stack_t *stack)
{
    free(stack->buffer);
    free(stack);
}
void StackPush(stack_t *stack, const void *item)
{
        assert(stack->top < stack->capacity);
        memcpy((char *)stack->buffer + (stack->top * stack->element_size), item, stack->element_size);
        ++stack->top;
}
void StackPop(stack_t *stack)
{
    assert(stack->top != 0);
    --stack->top;
}

void *StackPeek(stack_t *stack)
{
    assert(stack->top != 0);
    return (void*)((char *)stack->buffer + ((stack->top - 1) * stack->element_size));
}

int StackIsEmpty(const stack_t *stack)
{
    return 0 == stack->top ? 1 : 0;
}

size_t StackSize(const stack_t *stack)
{
    return stack->top;
}

size_t StackCapacity(const stack_t *stack)
{
    return stack->capacity;
}
