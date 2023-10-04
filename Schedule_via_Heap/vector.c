/*************************************************
* developer: Aviv Saraf
* reviewer: Neviu
* date: 
*
**************************************************/
  
#include <assert.h>   
#include <string.h>   /*memcpy*/
#include <stdlib.h>   /*malloc*/

#include "vector.h"

#define GROWTH_FACTOR (2)
#define QUARTER_FACTOR (4)
#define SUCCESS (0)
#define FAILURE (1)

struct vector
{
	void *data;
	size_t element_size;
	size_t capacity;
	size_t size;
};

vector_t *VectorCreate(size_t capacity, size_t element_size)
{
  vector_t *vector = NULL;
  
  assert(0 != element_size);
  
  (void) element_size;
  vector = (vector_t*)malloc(sizeof(vector_t));
  if (NULL == vector)
  {
    return vector;
  }

  vector -> data = malloc (capacity * sizeof(void *));
  if (NULL == vector -> data)
  {
    free(vector);
    vector = NULL;
  }
  else
  {
    vector -> element_size = sizeof(void *);
    vector -> capacity = capacity;
    vector -> size = 0;
  }
  
  return vector;
}

void VectorDestroy(vector_t *vector)
{
    assert(NULL != vector);
    
    free(vector -> data);
    free(vector);
}

int VectorReserve(vector_t *vector, size_t reserve_size)
{
    vector_t *temp = NULL;
    int ans = FAILURE;
    
    assert(NULL != vector);
    
    if (vector -> capacity < reserve_size)
    {
      temp = realloc(vector -> data, reserve_size * vector -> element_size);
      if (NULL != temp)
      {
        vector -> data = temp;
        vector -> capacity = reserve_size;
        ans = SUCCESS;
      }
    }
    return ans;
}

int VectorPushBack(vector_t *vector, const void *element)
{
    int ans = SUCCESS;
    
    assert(NULL != vector);
    assert(NULL != element);
    
    *(void**)((char*)vector -> data + (vector -> size * vector -> element_size)) = (void*)element;

    vector -> size += 1;
    
    if (vector -> size == vector -> capacity)
    {
        ans = VectorReserve(vector, vector -> capacity * GROWTH_FACTOR);
    }
    
    return ans;
}

void VectorPopBack(vector_t *vector)
{    
    vector_t *temp = NULL;
    
    assert(NULL != vector);
    assert(0 < (vector -> size));
    
    vector -> size -= 1;
    
    if (vector -> size <= (vector -> capacity / QUARTER_FACTOR))
    {
        temp = realloc(vector -> data,  (vector -> capacity / GROWTH_FACTOR) * vector -> element_size);
        if (NULL != temp)
        {
          vector -> data = temp;
          vector -> capacity /= GROWTH_FACTOR;
        } 
    }
}

void *VectorGetAccessToElement(const vector_t *vector, size_t index)
{   
    assert(NULL != vector);
    assert(0 < (vector -> size));

    return (void*)((char*)vector -> data + (index * vector -> element_size));
}

int VectorShrinkToSize(vector_t *vector)
{
    int ans = FAILURE;
    vector_t *temp = NULL;
    
    assert(NULL != vector);
    
    temp = realloc(vector -> data,  ((vector -> size + 1) * vector -> element_size));
    if (NULL != temp)
    {
      vector -> data = temp;
      vector -> capacity = vector -> size + 1;
      ans = SUCCESS;
    } 

    return ans;
}

size_t VectorCapacity(const vector_t *vector)
{
    assert(NULL != vector);
    return (vector -> capacity);
}

size_t VectorSize(const vector_t *vector)
{
    assert(NULL != vector);
    return vector -> size;
}

int VectorIsEmpty(const vector_t *vector)
{
    assert(NULL != vector);
    return (vector -> size == 0? 1: 0);
}









