#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#include <stddef.h> /* size_t */

#include "sortlist.h" 

typedef struct PQueue pq_t;


/*****************************************************************************/
/*
Description: Create a priority queue.
Arguments: 
compare - valid pointer to a comparison function 

Return: A pointer to the created queue.

Time complexity: O(1).
Space complexity: O(1).
*/

pq_t *PQCreate(int (*compare)(const void *,const void *));

/*****************************************************************************/
/*
Description: Destroy a priority queue.
Arguments: 
pq - valid pointer to a priority queue

Return: none

Time complexity: O(1).
Space complexity: O(1).
*/

void PQDestroy(pq_t *pq);

/*****************************************************************************/
/*
Description: Insert an element into the priority queue.
Arguments:
pq - valid pointer to a priority queue
data - valid pointer to data

Return: SUCCESS (0) / FAIL 

Time complexity: O(1).
Space complexity: O(1).
*/

int PQEnqueue(pq_t *pq, void *data);

/*****************************************************************************/
/*
Description: Insert an element into the priority queue.
Arguments:
pq - valid pointer to a priority queue

Return: pointer to data of removed element

Time complexity: O(1).
Space complexity: O(1).
*/

void *PQDequeue(pq_t *pq);

/***********************************************************************/
/*
Description: Look at value of queue with highest priority.
Behavior is undefined if queue is empty.
Arguments: 
pq - valid pointer to a priority queue

Return:
pointer to data

Time complexity: O(1)
Space complexity: O(1)
*/
void *PQPeek(const pq_t *pq);

/*****************************************************************************/
/*
Description: Erase an element from the queue.
Arguments:
pq - valid pointer to a priority queue
is_match - valid pointer to a match function
param - valid pointer to a parameter

Return: 
pointer to data of erased element or NULL if not found

Time complexity: O(n).
Space complexity: O(1).
*/

void *PQErase(pq_t *pq, int (*is_match)(const void *, const void *), void *param);

/*****************************************************************************/
/*
Description: Empty the queue.
Arguments:
pq - valid pointer to a priority queue

Return: none

Time complexity: O(n).
Space complexity: O(1).
*/

void PQClear(pq_t *pq);
/*****************************************************************************/
/*
Description: Count elements in the queue. 
Arguments:
pq - valid pointer to a priority queue

Return: number of elements

Time complexity: O(n).
Space complexity: O(1).
*/

size_t PQSize(const pq_t *pq);

/*****************************************************************************/
/*
Description: Count elements in the queue. 
Arguments:
pq - valid pointer to a priority queue

Return:
1 if queue is empty
0 if queue is not empty

Time complexity: O(1).
Space complexity: O(1).
*/
int PQIsEmpty(const pq_t *pq);

#endif /* __PQUEUE_H__ */
