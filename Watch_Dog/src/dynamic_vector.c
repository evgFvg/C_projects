/**********************
* developer: Neviu Mamu
* reviewed by: Mor Zibel 
***********************/

#include <stdlib.h>				/* malloc ,realloc ,free */
#include "dynamic_vector.h"	/* my liabry */
#include <assert.h>				/* asserts on pointers */
#include <string.h>				/* memcpy */
#include <stdio.h>				/* perorr */

#define GROWTH_FACTOR (2)
#define ELEMENTS_BYTE_SIZE ((*vector).capacity * (*vector).element_size)
#define SUCCESS (0)
#define FAILURE (1)
#define SHRINK_FACTOR (4)
#define ONE_EXTRA (1)

struct vector
{
	void *data;
	size_t element_size;
	size_t capacity;
	size_t size;
};


vector_t *VectorCreate(size_t capacity, size_t element_size)
{
	
	vector_t *new_vector_t = (vector_t*)malloc(sizeof(vector_t));
	
	if(NULL == new_vector_t)
	{ 
		perror("fail to allocate memory for struct\n");
		return NULL;
	}
	
	(*new_vector_t).data = (void *)malloc(capacity * element_size);
	
	if(NULL == (*new_vector_t).data)
	{
		perror("fail to allocate memory for vector\n");
		free(new_vector_t);
		return NULL;
	}	
	
	(*new_vector_t).capacity = capacity;
	(*new_vector_t).size = 0;
	(*new_vector_t).element_size = element_size;
	
	return new_vector_t;
}


void VectorDestroy(vector_t *vector)
{
	free((*vector).data);
	memset(vector, 0, sizeof(vector_t));
	free(vector);
}


int VectorPushBack(vector_t *vector, const void *element)
{
	char *tmp = NULL;
	int check = SUCCESS;
	
	assert(NULL != vector);
	assert(NULL != element);

	tmp =(char *) (*vector).data + (*vector).size * (*vector).element_size;
	
	memcpy(tmp, element, (*vector).element_size);
	
	++(*vector).size;
	
	if((*vector).size == (*vector).capacity)
	{
		tmp = (void *) realloc((*vector).data ,GROWTH_FACTOR * ELEMENTS_BYTE_SIZE);
		if(NULL != tmp)
		{
			(*vector).data = tmp;
			(*vector).capacity *= GROWTH_FACTOR;
		}
		else 
		{
			check = FAILURE;
		}	
	}
	
	return check;
}


void VectorPopBack(vector_t *vector)
{
	void *tmp = NULL;
	
	assert(NULL != vector);
	
	--(*vector).size;
	
	if((*vector).size <= ((*vector).capacity / SHRINK_FACTOR))
	{
		tmp = (void*) realloc((*vector).data ,ELEMENTS_BYTE_SIZE / GROWTH_FACTOR);
		if(NULL != tmp) 
		{
			(*vector).data = tmp;
			(*vector).capacity /= GROWTH_FACTOR;
		}
	}
}


void *VectorGetAccessToElement(const vector_t *vector, size_t index)
{
	char *tmp = NULL;
	
	assert(NULL != vector);
	
	tmp = (char *)(*vector).data + (index * (*vector).element_size);
	
	return (void *)tmp;
}


int VectorReserve(vector_t *vector, size_t reserve_size)
{
	void *tmp = NULL;
	int check = SUCCESS;
	
	assert(NULL != vector);	
	
	if ( reserve_size > (*vector).capacity)
	{
		tmp = (void*)realloc((*vector).data , (*vector).element_size * reserve_size);
		
		 if (NULL != tmp)
		 {
			 (*vector).data = tmp;
		       	 (*vector).capacity = reserve_size;
		 }
		 else
		 {
		 	 check = FAILURE;
		 }
	}
	
	return check;
}


int VectorShrinkToSize(vector_t *vector)
{
	void *tmp = NULL;
	int check = FAILURE;
	
	assert(NULL != vector);
	
	tmp = (void*)realloc((*vector).data, ((*vector).element_size * (*vector).size) + ONE_EXTRA);
	if(NULL != tmp)
	{
		(*vector).data = tmp;
		(*vector).capacity = (*vector).size + ONE_EXTRA;
		check =  SUCCESS;
	}
	
	return check;
}


int VectorIsEmpty(const vector_t *vector)
{
	assert(NULL != vector);

	return ((*vector).size == 0) ? 1 : 0; 

}


size_t VectorCapacity(const vector_t *vector)
{
	assert(NULL != vector);
	
	return (*vector).capacity;
}


size_t VectorSize(const vector_t *vector)
{
	assert(NULL != vector);
	
	return (*vector).size;
}



