#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>/*size_t*/
typedef struct vector vector_t;

/*
* Create Description:
*	creates new Dynamic Vector data structure.
*	the vector will be able to contain capacity number of elements of size item_size.
*
* @Params:
*	capacity - number of possible elements in the vector.
*	element_size - size of one element in bytes.
*
* @Returns:
*	pointer to a vector that can be used to access vector functions.
*
* @Complexity
*	Time: Amortized O(1)
*/
vector_t *VectorCreate(size_t capacity, size_t item_size);

/*
* Description:
*	realeases allocated memory for vector and all it's data.
*
* @Params:
*	vector - pointer to a vector to destroy.
*
* @Returns:
*	void
*
* @Complexity
*	Time: O(1)
*/
void VectorDestroy(vector_t *vector);

/* 
* VectorPushBack Description:
* 	pushes an element to the end of the vector
*	Reallocation function will be called if there is only one more storage place left.
*	In case the realocatin failed the element will be pushed to the vector anyway. 
*
* @param:
*	vector 	 - pointer to the vector to add an item to.	 
*	element - A pointer to the item to add.
*			
*
* @Return
*	Returns 0 if realocation of aditional memory succeded .
*       Returns 1 if realocation of aditional memory failed.
*	additional calls to this function will result an undefined behavior.
*
* @Complexity
*	Time: Amortized O(n) if reallocation is required / Amortized O(1) if not
*	
*/
int VectorPushBack(vector_t *vector, const void *element);

/* 
* PopBack Description
*	pops an element from the end of the vector
*	the capacity might be reduced by an internal growth factor.
*	popping an element from an empty vector causes an undefiend behaviour.
*
* @param:
*	vector - pointer to the vector.	 
*			
* @Return
*	void.
*
* @Complexity
*	Time: Amortized O(n) if reallocation is required / Amortized O(1) if not
*/
void VectorPopBack(vector_t *vector);

/* 
* VectorGetAccessToElement Description:
* 	Returns a pointer to the element by a given index.
*	The pointer returned by the function might be invalidated by either Pop or Push function calls.
*
* @param:
*	vector - pointer to the vector to read the element from. 
*       index - Index to the element in vector to read.
*			
* @Return
*	A pointer to the element stored in a given index. 
*
* @Complexity
*	Time: O(1)
*/
void *VectorGetAccessToElement(const vector_t *vector, size_t index);

/* 
* VectorReserve Description:
* 	Increasing the capacity of the vector to the given 'new_capacity'.
*	if the given new_capacity is less than the current one the data info will be lost.
*
* @param:
*	vector  -  pointer to the vector with the capacity to be changed. 
*	new_capacity -  the new capacity to set the vector to.
*
* @Return
*	returns 0 if the allocation was successful. 
*	returns 1 if failed to increase size.
*
* @Complexity
*	Time: O(n)
*/
int VectorReserve(vector_t *vector, size_t new_capacity);

/* 
* VectorShrinkToSize Description:
* Shrinks the capacity of a vector's capacity to the current number of elements + 1.
*
* @param:
*	vector - pointer to the vector that need to be shrinked.
*
* @Return
*	returns 0 if the allocation was successful, 
*	returns 1 if failed to shrink to size.
*
* @Complexity
*	Time: O(n)
*/
int VectorShrinkToSize(vector_t *vector);

/*
* 
* VectorCapacity Description:
* 	Checks the vector capacity.
*
* @param:
*	vector - The vector to check the capacity in;
*
* @Return
*	returns the number of elements the vector can currently store.
*
* @Complexity
*	Time: O(1)
*/
size_t VectorCapacity(const vector_t *vector);

/*
* 
* VectorSize Description:
* 	Checks the current number of elements in the vector.
*
* @param:
*	vector - The vector to check its number of elements. 
*
* @Return
*	returns the current number of elements.
*
* @Complexity
*	Time: O(1)
*/
size_t VectorSize(const vector_t *vector);

/*
* 
* VectorIsEmpty Description:
* 	Checks if the Vector is empty.
*
* @param:
*	vector - The vector to check if is empty. 
*
* @Return
*	returns 1 if empty.
*	returns 0 if not.
*
* @Complexity
*	Time: O(1)
*/
int VectorIsEmpty(const vector_t *vector);


#endif
