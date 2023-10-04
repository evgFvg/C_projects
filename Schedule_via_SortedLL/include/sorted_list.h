#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__

#include <stddef.h>
#include "dll.h"

typedef struct sorted_linked_list sol_t;

typedef struct sol_iterator iterator_st;

typedef int (*cmp_func_st)(const void *data1, const void *data2);

struct sol_iterator
{
    iterator_t DLLIterator;
#ifndef NDEBUG
    dll_t *dll;
#endif
};

/*
* SortedListCreate description:
* 	Creates a sorted List. 
* 	The list is sorted according to a compare function 'user_func'.
*
* @param:
* 	user_func - pointer to a compare function which fits to the temlpate 'int user_func(const void *data1, const void *data2)'
* 
* @return:
* 	Returns a pointer to the created sorted linked list.
*	If any problem occured while creation of the list, the function returns NULL
*
* complexity
* 	O(1)
*
*/
sol_t *SortedListCreate(cmp_func_st user_func);

/*
* SortedListDestroy description:
* 	Destroys sorted linked list and releases the resources involved
*
* @param:
* 	sol - Pointer to a sorted linked list
* 
* @return
* 	void
*
* complexity
* 	O(n)
*
*/
void SortedListDestroy(sol_t *sol);

/*
* SortedListIsEmpty description:
* 	Checks if list is empty
*
* @param:
* 	sol - Pointer to a sorted linked list
* 
* @return:
* 	Returns 1 if empty, 0 if not
*
* complexity
* 	O(1)
*
*/
int SortedListIsEmpty(const sol_t *sol);

/*
* SortedListSize description:
* 	Returns a number of elements in the list
*
* @param:
* 	sol - Pointer to a sorted linked list
* 
* @return:
* 	Returns number of elements in linked list
*
* @Complexity
* 	O(n)
*
*/							
size_t SortedListSize(const sol_t *sol);

/*
* SortedListInsert Description:
*	Inserts a node to the speciic position in the list, defined by the comapre function
*
* @Params:
*	sol - Pointer to a sorted linked list
*	data - pointer to the value for the inserted node.
*
* @Returns:
*	returns an iterator to the inserted node.
*	In case of failure returns iterator to the tail node.
*	if 'sol' is invalid, the behavior of the function is undefined.
*
* @Complexity
*	Time: O(n)
*/
iterator_st SortedListInsert(sol_t *sol, void *data); /*reuse Find*/

/*
*  SortedListRemove Description:
*	Removes a node pointed by the given iterator.
*
* @Params:
*	iterator - iterator to the node to be removed.
*
* @Returns:
*	returns an iterator to the next node in the sorted linked list.
*  	if "iterator" is invalid, the behavior of the function is undefined
*  	In case of removing tail, the behaviour of the function is undefined.
*	In case of removing head ( by using BeginIter() and then PrevIter( )), the behaviour of the function is undefined.
* 	if the list is empty the behavior is undefined.
*	
* @Complexity
*	Time: O(1)
*/
iterator_st SortedListRemove(iterator_st iterator);

/*
*  SortedListPopBack Description:
*  	Removes the last node in the list and returns a pointer to the data saved in the removed node
*
* @Params:
*	sol - Pointer to a sorted linked list
*
* @Returns:
*	returns pointer to data in the removed node
*	
* @Complexity
*	Time: O(1)
*/
void *SortedListPopBack(sol_t *sol);

/*
*  SortedListPopFront Description:
*  	Removes the first node in the list and returns a pointer to the data saved in the removed node
*
* @Params:
*	sol - Pointer to a sorted linked list
*
* @Returns:
*	returns pointer to data in the removed node
*	
* @Complexity
*	Time: O(1)
*/
void *SortedListPopFront(sol_t *sol);

/*
* SortedListFindIf Description:
*	searches the sorted list according to is_match_t function in the selected range (inclusive 'from' to exclusive 'to') with a given parameter.
*
* @Params:
*	from - pointer to the start of the range in the sorted linked list.
*	to - pointer to the end of the range in the sorted linked list.
*	match_func - pointer to a given is_match_t function.
*	param - paramter to find.
*
* @Returns:
*	returns an iterator to the first matching node.
*  	 In case of fail returns iterator to "to".
*  	 if "iterator" is invalid, the behavior of the function is undefined
*
* @Complexity
*	Time: O(n)
*/
iterator_st SortedListFindIf(iterator_st from, iterator_st to, is_match_t user_func, void *param); 

/*
* SortedListFind Description:
*	Searches the sorted list according to the cmp_func in 'sol' in the selected range (inclusive 'from' to exclusive 'to') with a given parameter.
*
* @Params:
*	from - pointer to the start of the range in a sorted linked list.
*	to - pointer to the end of the range in a sorted linked list.
*	sol - Pointer to a sorted linked list
*	to_find - paramter to compare.
*
* @Returns:
*	returns an iterator to the first matching node.
* 	In case of fail returns iterator to "to".
* 	if "iterator" is invalid, the behavior of the function is undefined
*
* @Complexity
*	Time: O(n)
*/
iterator_st SortedListFind(sol_t *sol, iterator_st from, iterator_st to, const void *to_find);

/*
* SortedListForEach Description:
*	Execute the given action_func_t on a given range of the sorted list (inclusive 'from' to exclusive 'to') with a given parameter.
*
* @Params:
*	from - pointer to the start of the range in the linked list.
*	to - pointer to the end of the range in the linked list.
*	action_func - pointer to a given action function sent by user.
*	param - paramter for the action function.
*
* @Returns:
*	returns action_function status. 
*  	0 if succeeded. 
*   	if one of the operations of the action func fails returns value different from 0.
*  	if "iterator" is invalid then - the behavior of the function is undefined.
*
* @Complexity
*	Time: O(n)
*/
int SortedListForEach(iterator_st from, iterator_st to, action_func_t user_func, void *param); /*assert from to*/
	
/*
* SortedListGetData Description:
*	gets the value of a node pointed by the given iterator.
*
* @Params:
*	iterator - iterator  points to a node.
*
* @Returns:
*	returns pointer to the data
*   	if "iterator" is invalid then - the behavior of the function is undefined.
*
* @Complexity
*	Time: O(1)
*/																					
void *SortedListGetData(iterator_st iterator);

/*
* SortedListBeginIter Description:
* 	returns the iterator that points to the first node of the sorted list.
* 	if list is empty, returns tail node.
*
* @Params:
*	sol - pointer to the sorted list.
*
* @Returns:
*	returns an iterator to the first node
*	if "sol" is invalid then the behavior of the function is undefined
*	Any actions with the Iterators pointed to tail or head can cause undefined behaviour.
*
* @Complexity
*	Time: O(1)
*/																									
iterator_st SortedListBeginIter(const sol_t *sol);	
	
/*
* SortedListEndIter Description:
*	returns the iterator that points to the last node of the sorted list.
*
* @Params:
*	sol - pointer to the sorted list.
*
* @Returns:
*   	if "sol" is invalid then the behavior of the function is undefined
*	returns the iterator that points to the end of the linked list.
	Any changes with the returned value of the function can occur undefined behaviour
*
* @Complexity
*	Time: O(1)
*/																								
iterator_st SortedListEndIter(const sol_t *sol);

/*
* SortedListNextIter Description:
*	returns the iterator that points to the next node.
*
* @Params:
*	iterator - pointer to the current node.
*   	if "iterator" is invalid then  the behavior of the function is undefined
	Any actions with the Iterators pointed to tail or head can cause undefined behaviour.
*
* @Returns:
*	returns the iterator that points to the next node.
*
* @Complexity
*	Time: O(1)
*/																						
iterator_st SortedListNextIter(iterator_st iterator);	
												
/*
* SortedListPrevIter Description:
*	returns the iterator that points to the previous node.
*
* @Params:
*	iterator - pointer to the current node.
*   if "iterator" is invalid then  the behavior of the function is undefined
*
* @Returns:
*	returns the iterator that points to the next node.
*	Any actions with the Iterators pointed to tail or head can cause undefined behaviour.
*
* @Complexity
*	Time: O(1)
*/																		
iterator_st SortedListPrevIter(iterator_st iterator);	

/*
* SortedListIsSameIter Description:
*	Compares two iterators
*
* @Params:
*	iter1 - iterator points to a node in a sorted list.
*	iter2 - iterator points to a node in a sorted list.
*
* @Returns:
*	returns 1 if they are equal.
	returns 0 otherwise.
*
* @Complexity
*	Time: O(1)
*/																	
int SortedListIsSameIter(iterator_st iter1, iterator_st iter2);	

/*
* SortedListMerge Description:
*	Combines two sorted lists into one sorted list, sorted according to the same compare function.
*	Lists must be sorted according to the same compare function.
*	src_list will be emptied but not destroyed.
*
* @Params:
*	dest_sol - pointer to a sorted list the two lists will merge to.
*	src_sol - pointer to a sorted list to be merged with dest_sol.
*
* @Returns:
*	returns pointer to dest_sol.
	incase two lists are not sorted according to the same compare function the behavior is undefined.
*
* @Complexity
*	Time: O(n+m)
*/																				
sol_t *SortedListMerge(sol_t *dest_sol, sol_t *src_sol);																		

#endif
