/********************
* Dev: Evgenii
* Rev: Arik
* Date: 19.2.2023
*********************/
/*-----------------------------includes---------------------------------------*/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <stddef.h> /* size_t */
#include "sorted_list.h"
#include <stdio.h>
/*-----------------------------structs----------------------------------------*/
struct sorted_linked_list
{
	dll_t *list;
	cmp_func_st user_func;
};

typedef struct
{
	cmp_func_st cmp;
	void* param;

}wrapper_t;

/*-----------------------------Static Functions-------------------------------*/
static int IsMatch( const void *data, const void *wd);
static int IsBigger( const void *data, const void *wd);

/*-----------------------------Functions--------------------------------------*/
sol_t *SortedListCreate(cmp_func_st user_func)
{
	sol_t *sorted_list = NULL;
	
	assert(NULL != user_func);
	
	sorted_list = malloc(sizeof(sol_t));
	if(NULL == sorted_list)
	{
		return NULL;
	}
	
	sorted_list -> list = DLLCreate();
	if(NULL == sorted_list->list)
	{
		free(sorted_list);
		return NULL;
	}
	
	sorted_list -> user_func = user_func;
	
	return 	sorted_list;
}

void SortedListDestroy(sol_t *sol)
{
	assert(NULL != sol);
	
	DLLDestroy(sol -> list);
	free(sol);
	sol = NULL;
}

iterator_st SortedListBeginIter(const sol_t *sol)
{
	iterator_st ret;
	
	assert(NULL != sol);
	
	ret.DLLIterator = DLLBeginIter(sol->list);
	
	#ifndef NDEBUG
	ret.dll = sol -> list;
	#endif
	
	return ret;
}

iterator_st SortedListEndIter(const sol_t *sol)
{
	iterator_st ret;
	
	assert(NULL != sol);
	
	ret.DLLIterator = DLLEndIter(sol->list);
	
	#ifndef NDEBUG
	ret.dll = sol -> list;
	#endif
	
	return ret;
}

iterator_st SortedListNextIter(iterator_st iterator)
{
	iterator_st ret;
	
	assert(NULL != iterator.DLLIterator);
	
	ret.DLLIterator = DLLNextIter(iterator.DLLIterator);
	
	#ifndef NDEBUG
	ret.dll = iterator.dll;
	#endif
	
	return ret;
}

iterator_st SortedListPrevIter(iterator_st iterator)
{
	iterator_st ret;
	
	assert(NULL != iterator.DLLIterator);
	
	ret.DLLIterator = DLLPrevIter(iterator.DLLIterator);
	
	#ifndef NDEBUG
	ret.dll = iterator.dll;
	#endif
	
	return ret;
}

int SortedListIsEmpty(const sol_t *sol)
{
	assert(NULL != sol);
	
	return DLLIsEmpty(sol -> list);
}

size_t SortedListSize(const sol_t *sol)
{
	assert(NULL != sol);
		
	return DLLCount(sol -> list);
}

iterator_st SortedListInsert(sol_t *sol, void *data)
{	
	iterator_st begin = {0};
	iterator_st end = {0};
	iterator_st before = {0};
	wrapper_t wrapper = {0};
	
	assert(NULL != sol);
		
	begin = SortedListBeginIter(sol);
	end = SortedListEndIter(sol);
	
	wrapper.cmp = sol -> user_func;
	wrapper.param = data;
	
	before = SortedListFindIf(begin, end, IsBigger, &wrapper);	
	before.DLLIterator = DLLInsert(before.DLLIterator, data);
		
	#ifndef NDEBUG
	before.dll = sol -> list;
	#endif
	
	return before;	
}

iterator_st SortedListRemove(iterator_st iterator)
{
	iterator_st removed;
	
	assert(NULL != iterator.DLLIterator);
	
	removed.DLLIterator = DLLRemove(iterator.DLLIterator);
	
	#ifndef NDEBUG
	removed.dll = iterator.dll;
	#endif
	
	return removed;
}

int SortedListIsSameIter(iterator_st iter1, iterator_st iter2)
{
	assert(NULL != iter1.DLLIterator);
	assert(NULL != iter2.DLLIterator);
	
	return DLLIsSameIter(iter1.DLLIterator, iter2.DLLIterator);
}

void *SortedListPopFront(sol_t *sol)
{	
	assert(NULL != sol);
		
	return DLLPopFront(sol -> list);
}

void *SortedListPopBack(sol_t *sol)
{
	assert(NULL != sol);

	return DLLPopBack(sol -> list);;
}

void *SortedListGetData(iterator_st iterator)
{
	assert(NULL != iterator.DLLIterator);
		
	return DLLGetData(iterator.DLLIterator);	
}

iterator_st SortedListFind(sol_t *sol, iterator_st from, iterator_st to, const void *to_find)
{	
	iterator_st ret = {0};
	wrapper_t wrapper = {0};

	assert(NULL != sol);
	assert(NULL != from.DLLIterator);
	assert(NULL != to.DLLIterator);
	assert(from.dll == to.dll);
	
	wrapper.param =(void *)to_find;
	wrapper.cmp = sol -> user_func;
	
	ret.DLLIterator = DLLFind(from.DLLIterator, to.DLLIterator, IsMatch, &wrapper);
	
	#ifndef NDEBUG
	ret.dll = sol -> list;
	#endif	
	
	return ret;
}

iterator_st SortedListFindIf(iterator_st from, iterator_st to, is_match_t user_func, void *param)
{
	iterator_st ret = {0};
	
	assert(NULL != from.DLLIterator);
	assert(NULL != to.DLLIterator);
	assert(NULL != user_func);
	assert(from.dll == to.dll);
	
	ret.DLLIterator = DLLFind(from.DLLIterator, to.DLLIterator, user_func, param);
	
	#ifndef NDEBUG
	ret.dll = to.dll;
	#endif
	
	return ret;
}

int SortedListForEach(iterator_st from, iterator_st to, action_func_t user_func, void *param)
{
	assert(NULL != from.DLLIterator);
	assert(NULL != to.DLLIterator);
	assert(NULL != user_func);
	assert(from.dll == to.dll);
		
	return DLLForEach(from.DLLIterator, to.DLLIterator, user_func, param);
}

sol_t *SortedListMerge(sol_t *dest_sol, sol_t *src_sol)
{
	iterator_st dest_to = {0};
	iterator_st dest_before = {0};
	iterator_st src_from = {0};
	iterator_st src_to = {0};
	wrapper_t wrapper = {0};
	
	assert(NULL != dest_sol);
	assert(NULL != src_sol);
	
	dest_before = SortedListBeginIter(dest_sol);
	dest_to = SortedListEndIter(dest_sol);
	
	src_from = SortedListBeginIter(src_sol);
	src_to = SortedListEndIter(src_sol);
	
	wrapper.cmp = dest_sol -> user_func;

	while (!SortedListIsEmpty(src_sol))
	{
		wrapper.param =  SortedListGetData(src_from);
		
		dest_before = SortedListFindIf(dest_before, dest_to, IsBigger, &wrapper);
		
		if (SortedListIsSameIter(dest_before, dest_to))
		{
			src_to = SortedListEndIter(src_sol);
		}
		else
		{
			wrapper.param = SortedListGetData(dest_before);
			src_to = SortedListFindIf(src_from, src_to, IsBigger, &wrapper);
		}
			
		DLLSplice(dest_before.DLLIterator, src_from.DLLIterator, src_to.DLLIterator);
		
		src_from = SortedListBeginIter(src_sol);
		src_to = SortedListEndIter(src_sol);
	}
	
	return dest_sol;

}

/*------------------------------------Static Function-------------------------*/

static int IsMatch(const void *data, const void *wd)
{
	wrapper_t *wrapper =(wrapper_t*)wd;
	
	return (0 == (wrapper -> cmp(data, wrapper -> param)));
}
static int IsBigger( const void *data, const void *wd)
{
	wrapper_t *wrapper =(wrapper_t*)wd;
	
	return (0 < (wrapper -> cmp(data, wrapper -> param)));
}



