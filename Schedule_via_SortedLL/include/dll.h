#ifndef __DLL_H__
#define __DLL_H__

#include <stddef.h> /*size_t*/

typedef struct node node_t;	
typedef struct node *iterator_t;
typedef struct doubly_linked_list dll_t;

typedef int (*action_func_t)(void *, void *); 
typedef int (*is_match_t)(const void *, const void *);

/*
* DLLCreate description:
* 	Creates a doubly linked list
*
* @param:
* 	none
* 
* @return:
* 	Returns a pointer to the created doubly likned list.
*
* complexity
*	Time O(1)
*/
dll_t *DLLCreate();

/*
* DLLDestroy description:
* 	Destroys doubly linked list
*
* @param:
* 	dll - Pointer to doubly linked list
* 
* @return:
* 	void
*
* complexity
* 	Time O(1)
*/
void DLLDestroy(dll_t *dll);

/*
* DLLIsEmpty description:
* 	Checks if list is empty
*
* @param:
* 	dll - Pointer to doubly linked list
* 
* @return:
* 	Returns 1 if empty, 0 if not
*
* complexity
* 	Time O(1)
*/
int DLLIsEmpty(const dll_t *dll);							

/*
* DLLCount description:
* 	Counts number of elements in the list
*
* @param:
* 	dll - Pointer to doubly linked list
* 
* @return:
* 	Returns number of elements in the linked list
*
* @Complexity
* 	Time O(n)
*/
size_t DLLCount(const dll_t *dll);

/*
* DLLInsert Description:
*	Inserts data provided by user before node pointed by the given iterator.
*
* @Params:
*	iterator - pointer to the node before which need to insert data.
*	data - pointer to the value for the insertion.
*
* @Returns:
*	returns the iterator to the inserted node.
*	In case of fail returns iterator to the tail node.
*	if "iterator" is invalid, the behavior of the function is undefined.
*
* @Complexity
*	Time: O(1)
*/
iterator_t DLLInsert(iterator_t iterator, void *data);

/*
*  DLLRemove Description:
*	Removes a node pointed by the given iterator.
*
* @Params:
*	iterator - iterator to node to be removed.
*
* @Returns:
*	returns an iterator to the next node in the linked list.
*   	if "iterator" is invalid, the behavior of the function is undefined.
*	In case of removing tail, the behaviour of the function is undefined.
*	In case of removing head ( by using DllBeginIter() and then DllPrevIter( )), the behaviour of the function is undefined.
* 	if the list is empty the behavior is undefined.
*
* @Complexity
*	Time: O(1)
*/
iterator_t DLLRemove(iterator_t iterator);

/*
*  DLLPushBack Description:
*  	Inserts data at the end of list.  
*
* @Params:
*	dll - Pointer to doubly linked list
*	data - Pointer to data to be inserted
*
* @Returns:
*	returns an iterator to the node that was inserted.
*
* @Complexity
*	Time: O(1)
*/
iterator_t DLLPushBack(dll_t *dll, void *data); 

/*
*  DLLPushFront Description:
*  	Inserts data at the beginning of list
*
* @Params:
*	dll - Pointer to doubly linked list
*	data - pointer to data to be inserted
*
* @Returns:
*	returns an iterator to the node that was inserted
*	
* @Complexity
*	Time: O(1)
*/
iterator_t DLLPushFront(dll_t *dll, void *data);

/*
*  DLLPopBack Description:
*  	Removes the last node in the list and returns a pointer to the data saved in the removed node
*
* @Params:
*	dll - Pointer to doubly linked list
*
* @Returns:
*	returns pointer to data in the removed node
*	if the list is empty the behavior is undefined.
*	
* @Complexity
*	Time: O(1)
*/
void *DLLPopBack(dll_t *dll);

/*
*  DLLPopFront Description:
* 	 Removes the last node in the list and returns a pointer to the data saved in removed node
*
* @Params:
*	dll - Pointer to doubly linked list
*
* @Returns:
*	returns pointer to data in the removed node
*	if the list is empty the behavior is undefined.
*	
* @Complexity
*	Time: O(1)
*/
void *DLLPopFront(dll_t *dll);

/*
* DLLFind Description:
*	Executes the given function on a given part of the linked list (inclusive 'from' to exclusive 'to') with a given parameter.
*
* @Params:
*	from - pointer to the start of the range in the linked list.
*	to - pointer to the end of the range in the linked list.
*	match_func - pointer to a given compare function.
*	param - paramter to compare to.
*
* @Returns:
*	returns an iterator to the matching node.
*   	In case of fail returns iterator to "to".
*   	if "iterator" is invalid, the behavior of the function is undefined
*
* @Complexity
*	Time: O(n)
*/
iterator_t DLLFind(iterator_t from, iterator_t to, is_match_t user_func, void *param); 
	 
/*
* DLLFind Description:
* 	Serching all nodes (inclusive 'from' to exclusive 'to') in the DLL containing data, that matches criteria, 
	provided by user as a compare function
*
* @Params:
*	dll_dest - pointer to an empty list given by user
*	from - pointer to the start of the range in the linked list.
*	to - pointer to the end of the range in the linked list.
*	match_func - pointer to a given compare function.
*	param - paramter to compare to.
*
* @Returns:
*	returns a linked list with nodes that fit to the provided criteria.
*   	In case of were no matches were found- returns dest as it been recieved.
*
* @Complexity
*	Time: O(n)
*/
dll_t *DLLMultiFind(dll_t *dll_dest, iterator_t from, iterator_t to, is_match_t user_func, void *param);

/*
* DLLForEach Description:
*	Executes the given function on a given part of the linked list (inclusive 'from' to exclusive 'to') with a given parameter.
*
* @Params:
*	from - pointer to the start of the range in the linked list.
*	to - pointer to the end of the range in the linked list.
*	action_func - pointer to a given action function provided by user.
*	param - paramter for the action function.
*
* @Returns:
*	returns action_function status. 
*  	0 if succeeded. 
*  	if one of the operations of the action func fails returns any value different from 0.
*  	if "iterator" is invalid then - the behavior of the function is undefined.
*
* @Complexity
*	Time: O(n)
*/
int DLLForEach(iterator_t from, iterator_t to, action_func_t user_func, void *param);
	
/*
* DLLSetData Description:
*	sets the value of a node pointed by the given iterator.
*
* @Params:
*	iterator - iterator pointed to the node.
*	data - the data to set in the node.
*	Using the function with iterator to tail can occur undefined behaviour
* 	Using the function with iterator to BeginIter() and then PrevIter( ), can occur undefined behaviour
*
* @Returns:
*	void
*
* @Complexity
*	Time: O(1)
*/	
void DLLSetData(iterator_t iterator, void *data);																								

/*
* DLLGetData Description:
*	gets the value of a node pointed by the given iterator.
*
* @Params:
*	iterator - iterator  pointed to a node.
*
* @Returns:
*	returns pointer to the data
*       if "iterator" is invalid then - the behavior of the function is undefined.
*	Using the function with iterator to tail can occur undefined behaviour
*	Using the function with iterator to BeginIter() and then PrevIter( ), can occur undefined behaviour
*
* @Complexity
*	Time: O(1)
*/
void *DLLGetData(iterator_t iterator);																												

/*
* DLLBeginIter Description:
*	returns an iterator to the first node in the linked list
*
* @Params:
*	dll - pointer to the linked list.
*
* @Returns:
*	returns an iterator to the first node.
*	If list is empty the functions behaviour if undefined.
*	if "dll" is invalid then the behavior of the function is undefined
*
* @Complexity
*	Time: O(1)
*/
iterator_t DLLBeginIter(const dll_t *dll);																											

/*
* DLLEndIter Description:
*	returns the iterator to the end of the linked list.
*
* @Params:
*	dll - pointer to the linked list.
*
* @Returns:
*	returns an iterator to the last node.
*	If list is empty the functions behaviour if undefined.
*   	if "dll" is invalid then  the behavior of the function is undefined
*	
* @Complexity
*	Time: O(1)
*/
iterator_t DLLEndIter(const dll_t *dll);

/*
* DLLNextIter Description:
*	returns the iterator that points to the next node.
*
* @Params:
*	iterator - pointer to the current node.
*	If list is empty the functions behaviour if undefined.
*	Using the function with the iterator to tail can occur undefined bahaviour.
*   	if "iterator" is invalid then  the behavior of the function is undefined
*
* @Returns:
*	returns the iterator that points to the next node.

*
* @Complexity
*	Time: O(1)
*/																									
iterator_t DLLNextIter(iterator_t iterator);																									

/*
* DLLPrevIter Description:
*	returns the iterator that points to the previous node.
*
* @Params:
*	iterator - pointer to the current node.
*	If list is empty the functions behaviour if undefined.
*	Using the function with the iterator to head can occur undefined bahaviour.
*   	if "iterator" is invalid then  the behavior of the function is undefined
*
* @Returns:
*	returns the iterator that points to the previous node.
*
* @Complexity
*	Time: O(1)
*/				
iterator_t DLLPrevIter(iterator_t iterator);																									

/*
* DLLIsSameIter Description:
*	Compares two iterators
*
* @Params:
*	iter1 - iterator to a node in a linked list.
*	iter2 - iterator to a node in a linked list.
*
* @Returns:
*	returns 1 if they are equal.
	returns 0 otherwise.
*
* @Complexity
*	Time: O(1)
*/
int DLLIsSameIter(iterator_t iter1, iterator_t iter2);																					

/*
* DLLSplice Description:
*	Takes a range of nodes from another DLL and inserts them before a specified position in the current DLL.
*
* @Params:
*	dest - iterator pointed to a node in destination linked list.
*	src_from - iterator pointed to the first node in the required range of source linked list. (inclusive)
*	src_to - iterator points to the last node in the required range of source linked list. (exclusive)
*
* @Returns:
*	returns iterator to the first copied element in dest linked list. 
*
* @Complexity
*	Time: O(1)
*/
iterator_t DLLSplice(iterator_t dest, iterator_t src_from ,iterator_t src_to);																		
	
#endif
