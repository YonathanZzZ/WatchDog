#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "pqueue.h"

enum
{
	SUCCESS = 0,
	FAILURE = 1,
	TRUE = 1,
	FALSE = 0
};

struct PQueue
{
	sort_list_t *pqueue;
	/* a pointer to a compare function is already included in the
	   sorted_list struct */
};

pq_t *PQCreate(int (*compare)(const void *,const void *))
{
	pq_t *pq = NULL;
	
	assert(compare);
	
	pq = (pq_t*)malloc(sizeof(pq_t));
	if(NULL == pq)
	{
		return (NULL);
	}
	
	pq->pqueue = SortListCreate((int (*)(void *, const void *))compare);
	if(NULL == pq->pqueue)
	{
		free(pq);
		
		return (NULL);
	}
	
	return (pq);
}

void PQDestroy(pq_t *pq)
{
	assert(pq);
	
	SortListDestroy(pq->pqueue);
	
	free(pq);
}

void *PQPeek(const pq_t *pq)
{
	assert(pq);
	assert(!PQIsEmpty(pq));
	/* get data of last item in list (queue) */
	return (SortListGetData(SortListPrev(SortListEnd(pq->pqueue))));
}

int PQIsEmpty(const pq_t *pq)
{
	assert(pq);
	
	return (SortListIsEmpty(pq->pqueue));
}

size_t PQSize(const pq_t *pq)
{
	assert(pq);
	
	return(SortListSize(pq->pqueue));
}

void *PQDequeue(pq_t *pq)
{
	assert(pq);
	
	return (SortListPopBack(pq->pqueue));
}

void PQClear(pq_t *pq)
{
	assert(pq);
	
	/* remove all items in the queue */
	while(!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
}

void *PQErase(pq_t *pq, int (*is_match)(const void *, const void *), void *param)
{
	sort_iter_t item_to_erase = {0};
	void *data_of_item_to_erase = NULL;
	
	assert(pq);
	assert(is_match);
	
	/* order of arguments for is_match: (const void *data, const void *param) */
	
	/* find item to erase */
	item_to_erase = SortListFindIf(SortListBegin(pq->pqueue), 
								   SortListEnd(pq->pqueue), (int(*)(void *, const void *))is_match, param);
								   
	/* if item was not found... */
	if(SortListIsEqual(item_to_erase, SortListEnd(pq->pqueue)))	
	{
		return (NULL);
	}   
	
	data_of_item_to_erase = SortListGetData(item_to_erase);
	SortListRemove(item_to_erase);
	
	return (data_of_item_to_erase);
}

int PQEnqueue(pq_t *pq, void *data)
{
	sort_iter_t insert_result = {0};
	assert(pq);
	
	insert_result = SortListInsert(pq->pqueue, data);
	
	/* insert returns end of list on failure */
	if(SortListIsEqual(insert_result, SortListEnd(pq->pqueue)))
	{
		return (FAILURE);
	}
	
	return (SUCCESS);
}
