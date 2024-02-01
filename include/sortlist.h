#ifndef __SORT_LIST_H__
#define __SORT_LIST_H__

#include <stddef.h> /* size_t */

#include "dlist.h" 

typedef struct SortList sort_list_t;

typedef struct SortIter
{
	dlist_iter_t iter;
	#ifndef NDEBUG
	sort_list_t *list;
	#endif /*NDEBUG*/
} sort_iter_t;

typedef int (*compare_t)(void *, const void *); /*comparative int*/
typedef int (*action_t)(void *, void *);
typedef int (*is_match_t)(void *, const void *); /*reuse name for boolean*/

/***********************************************************************/
/*
Description: create a sorted list
Arguments: func - valid pointer to a comparison function of compare_t type
Return: pointer to a list

Time complexity: O(1).
Space complexity: O(1).
*/

sort_list_t *SortListCreate(compare_t func);

/***********************************************************************/
/*
Description: destroy a list
Arguments: list - valid pointer to a list
Return: none

Time complexity: O(n).
Space complexity: O(1).
*/

void SortListDestroy(sort_list_t *list);

/***********************************************************************/
/*
Description: insert element into a list
Arguments: 
list - valid pointer to a list
data - valid pointer to data
Return: sorted-list iterator 

Time complexity: O(n).
Space complexity: O(1).
*/

sort_iter_t SortListInsert(sort_list_t *list, void *data);

/***********************************************************************/
/*
Description: remove element from a list
Arguments: current - valid iterator
Return: iterator to next element

Time complexity: O(1).
Space complexity: O(1).
*/

sort_iter_t SortListRemove(sort_iter_t current);

/***********************************************************************/
/*
Description: find first element in specified range that contains the data of "param" 
Arguments:
from - valid iterator to first element in range
to - valid iterator to last element in range ('to' is not included in the range)
list - valid sorted-list pointer
param - valid pointer to data
Return: iterator to found element, or to last in range if none found

Time complexity: O(n).
Space complexity: O(1).
*/

sort_iter_t SortListFind(sort_iter_t from, sort_iter_t to, sort_list_t *list, void *param);

/***********************************************************************/
/*
Description: run action function on all elements in specified range (exc. 'to') 
un-sorting list by changing data is undefined. 
Arguments: 
from - valid iterator to first element in range
to - valid iterator to last element in range ('to' is not included in the range)
action - valid pointer to function of type action_t
action_param - valid pointer to data used by the action function
Return: 
1 - success
0 - failure

Time complexity: O(n).
Space complexity: O(1).
*/

int SortListForEach(sort_iter_t from, sort_iter_t to, action_t action, void *action_param);

/***********************************************************************/
/*
Description: get number of elements in list
Arguments: list - valid pointer to a list
Return: number of elements

Time complexity: O(n).
Space complexity: O(1).
*/

size_t SortListSize(const sort_list_t *list);

/***********************************************************************/
/*
Description: check if list is empty
Arguments: list - valid pointer to a list
Return: 
1 if empty
0 if not empty

Time complexity: O(1).
Space complexity: O(1).
*/

int SortListIsEmpty(sort_list_t *list);

/***********************************************************************/
/*
Description: get iterator of next element
Arguments: current - valid iterator to element 
Return: iterator to next element.
behavior is undefined when used with end of list.

Time complexity: O(1).
Space complexity: O(1).
*/

sort_iter_t SortListNext(sort_iter_t current);

/***********************************************************************/
/*
Description: get iterator of previous element
Arguments: current - valid iterator to element 
Return: iterator to previous element.
behavior is undefined when used on first element
Time complexity: O(1).
Space complexity: O(1).
*/

sort_iter_t SortListPrev(sort_iter_t current);

/***********************************************************************/
/*
Description: get iterator to first element
Arguments: list - valid pointer to a list
Return: iterator to first element

Time complexity: O(1).
Space complexity: O(1).
*/

sort_iter_t SortListBegin(const sort_list_t *list);

/***********************************************************************/
/*
Description: get iterator to end of list (after the last element)
Arguments: list - valid pointer to a list
Return: iterator to end of list

Time complexity: O(1).
Space complexity: O(1).
*/

sort_iter_t SortListEnd(const sort_list_t *list);

/***********************************************************************/
/*
Description: get data of element
Arguments: current - valid iterator to element
Return: pointer to data of element

Time complexity: O(1).
Space complexity: O(1).
*/

void *SortListGetData(sort_iter_t current);

/***********************************************************************/
/*
Description: remove first element in list
Arguments: list - valid pointer to a list
Return: data of removed element 

Time complexity: O(1).
Space complexity: O(1).
*/

void *SortListPopFront(sort_list_t *list);

/***********************************************************************/
/*
Description: remove last element in list
Arguments: list - valid pointer to a list
Return: data of removed element 

Time complexity: O(1).
Space complexity: O(1).
*/

void *SortListPopBack(sort_list_t *list);

/***********************************************************************/
/*
Description: combine two lists into one. the resulting list (dest) is sorted and
			 src is empty.
Arguments: 
dest - valid pointer to a list
src - valid pointer to a list
Return: none.

Time complexity: O(n).
Space complexity: O(1).
*/

void SortListMerge(sort_list_t *dest, sort_list_t *src);

/***********************************************************************/
/*
Description: find first element in specified range that contains the data of "param" 
Arguments:
from - valid iterator to first element in range
to - valid iterator to last element in range ('to' is not included in the range)
is_match - valid pointer to a function of type is_match_t
param - valid pointer to data

Return: iterator to found element, or to last in range if none found

Time complexity: O(n).
Space complexity: O(1).
*/

sort_iter_t SortListFindIf(sort_iter_t from, sort_iter_t to, is_match_t is_match, void *param);

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

int SortListIsEqual(sort_iter_t iter1 , sort_iter_t iter2);

#endif /* __SORT_LIST_H__ */
