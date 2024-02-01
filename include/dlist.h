#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h> /* size_t */

typedef struct Node node_t;

typedef struct DList dlist_t;

typedef node_t *dlist_iter_t;

/***********************************************************************/
/*
Description: creates a doubly-linked list
Arguments: none
Return: 
pointer to a list, or NULL if it fails

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_t *DListCreate(void);

/***********************************************************************/
/*
Description: destroy (delete) a list
Arguments: list - a valid pointer to a list
Return: none

Time complexity: O(n).
Space complexity: O(1).
*/

void DListDestroy(dlist_t *list);

/***********************************************************************/
/*
Description: find the number of elements in a list
Arguments: list - a valid pointer to a list
Return: number of elements

Time complexity: O(n).
Space complexity: O(1).
*/

size_t DListSize(const dlist_t *list);

/***********************************************************************/
/*
Description: add an element into the list, placed before 'where'
Arguments: 
where - valid element iterator
data - valid pointer to data
Return: 
on success - pointer to newly created element
on failure - end of list

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListInsert(dlist_iter_t where, void *data);

/***********************************************************************/
/*
Description: remove an element from a list
Arguments: current - a valid iterator for an element
Return: iterator to next element

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListRemove(dlist_iter_t current);

/***********************************************************************/
/*
Description: get iterator to next element
Arguments: current - a valid element iterator
Return: iterator to next element

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListNext(dlist_iter_t current);

/***********************************************************************/
/*
Description: get iterator to previous element
Arguments:	current - a valid element iterator
Return: iterator to previous element

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListPrev(dlist_iter_t current);

/***********************************************************************/
/*
Description: check if list is empty or not
Arguments: list - valid pointer to a list
Return: 
1 if empty
0 if not empty

Time complexity: O(1).
Space complexity: O(1).
*/

int DListIsEmpty(const dlist_t *list);

/***********************************************************************/
/*
Description: find an element that contains "param" data in given range
Arguments:
from - valid iterator to first element in range
to - valid iterator to last element in range (excluded from range)
match_func - valid pointer to a function that checks if the data of an elemenet
matches the param.
param - valid pointer to parameter to be used by match_func
Return: iterator to first found element

Time complexity: O(n).
Space complexity: O(1).
*/

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, int (*match_func)(void *data, const void *param), const void *param) ;

/***********************************************************************/
/*
Description: perform action on data of all elements in range (excluding last one)
Arguments:	
from - valid iterator to first element in range
to - valid iterator to last element in range (excluded from range)
action_func - valid pointer to a function that manipulates data of elements
action_param - valid pointer to parameter to be used by action_func
Return: 

Time complexity: O(n).
Space complexity: O(1).
*/

int DListForEach(dlist_iter_t from, dlist_iter_t to, int(*action_func)(void *data, void *param), void *action_param);

/***********************************************************************/
/*
Description: change data of element
Arguments: 
current - valid iterator to element
data - valid pointer to data to be assigned
Return: none

Time complexity: O(1).
Space complexity: O(1).
*/

void DListSetData(dlist_iter_t current, void *data);

/***********************************************************************/
/*
Description: get data of element
Arguments: current - valid iterator to element
Return: pointer to data of element

Time complexity: O(1).
Space complexity: O(1).
*/

void *DListGetData(dlist_iter_t current);

/***********************************************************************/
/*
Description: check if two iterators are identical
Arguments: 
iter1 - valid iterator to element
iter2 - valid iterator to element 
Return: 
1 if iterators are equal
0 if iterators are not equal

Time complexity: O(1).
Space complexity: O(1).
*/

int DListIsEqual(dlist_iter_t iter1 , dlist_iter_t iter2);

/***********************************************************************/
/*
Description: get iterator to first element
Arguments: list - valid pointer to a list
Return: iterator to first element

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListBegin(const dlist_t *list);

/***********************************************************************/
/*
Description: get iterator to end of list (after the last element)
Arguments: list - valid pointer to a list
Return: iterator to end of list

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListEnd(const dlist_t *list);

/***********************************************************************/
/*
Description: insert element to last place in list
Arguments: list - valid pointer to a list, data - valid pointer to data
Return: 
on success - pointer to newly created element
on failure - iterator to end of list
Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListPushBack(dlist_t *list, void *data);

/***********************************************************************/
/*
Description: insert element to beginning of list
Arguments: list - valid pointer to a list, data - valid pointer to data
Return: 
on success - pointer to newly created element
on failure - iterator to end of list

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListPushFront(dlist_t *list, void *data);

/***********************************************************************/
/*
Description: remove first element in list
Arguments: list - valid pointer to a list
Return: data of removed node 
Time complexity: O(1).
Space complexity: O(1).
*/

void *DListPopFront(dlist_t *list);

/***********************************************************************/
/*
Description: remove last element in list
Arguments: list - valid pointer to a list
Return: data of removed node 
Time complexity: O(1).
Space complexity: O(1).
*/

void *DListPopBack(dlist_t *list);

/***********************************************************************/
/*
Description: transfer range of elements from one list to another
Arguments: 
where - valid iterator to element before which the elements in the range will be inserted
from - valid iterator to first element in range
to - valid iterator to last element in range. this element will not be inserted.

Return: iterator to the "from" element in its new location.

Time complexity: O(1).
Space complexity: O(1).
*/

dlist_iter_t DListSplice(dlist_iter_t where, dlist_iter_t from, dlist_iter_t to);

/***********************************************************************/
/*
Description: find elements containing "param" data in range and insert their 
iterators into an output list

Arguments:
from - valid iterator to first element in range
to - valid iterator to last element in range (the element "to" is not included)
match_func - valid pointer to a function that checks if the data of an elemenet
matches the param.
param - valid pointer to parameter to be used by match_func
output_list - pointer to empty list into which the found iterators will be inserted (as data)
Return: 
1 if elements are found and saved in output list. 0 otherwise.

Time complexity: O(n).
Space complexity: O(n).
*/

int DListMultiFind(dlist_iter_t from, dlist_iter_t to, int (*match_func)(void *data, const void *param), const void *param, dlist_t *output_list);

#endif /* __DLIST_H__ */
