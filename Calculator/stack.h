#ifndef STACK_H
#define STACK_H
#include<stddef.h> /*size_t*/
typedef struct stack stack_t;

/*creates a struct of typedef stack_t.*/
stack_t *StackCreate(size_t capacity, size_t element_size);

/*destorys the struct of typedef stack_t, and frees any dynamicly allocated memory.*/
void StackDestroy(stack_t *stack);

/*pushes item to struct of typedef stack_t.*/
void StackPush(stack_t *stack, const void *item);

/*pops out last item in the stack.*/
void StackPop(stack_t *stack);

/*Returnes a pointer to the top item of the stack. */
void *StackPeek(stack_t *stack);

/**checks if the stack is empty*/
int StackIsEmpty(const stack_t *stack);

/*returnes the number of items in the stack*/
size_t StackSize(const stack_t *stack);

/*returnes the maximum possible amount of items in the stack*/
size_t StackCapacity(const stack_t *stack);

#endif
