#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "dlist.h"

enum Status
{
	SUCCESS = 0,
	FAILURE = 1,
	TRUE = 1,
	FALSE = 0
};

struct Node 
{
	void *data;
	node_t *next;
	node_t *prev;
};

struct DList 
{
	node_t head;
	node_t tail;
};

static node_t *EndOfList(node_t *runner);
static void PointNodeToNext(node_t *curr_node, node_t *next_node);
static void PointNodeToPrev(node_t *curr_node, node_t *prev_node);

static void InitializeList(dlist_t* list)
{
	list->head.data = NULL;
	list->head.prev = NULL;
	list->head.next = &(list->tail);
	
	list->tail.data = NULL;
	list->tail.next = NULL;
	list->tail.prev = &(list->head);
}

dlist_t *DListCreate(void)
{
	dlist_t *list = NULL;
	
	list = (dlist_t*)malloc(sizeof(dlist_t));
	if (NULL == list)
	{
		return (NULL);
	}

	InitializeList(list);	

	return (list);
}

void DListDestroy(dlist_t *list)
{
	node_t *runner = NULL;
	node_t *node_to_remove = NULL;
	
	assert(NULL != list);
	
	runner = DListBegin(list);
	
	while(NULL != runner->next)
	{
		node_to_remove = runner;
		runner = runner->next;
		free (node_to_remove);
	}
	
	free(list);
}

dlist_iter_t DListInsert(dlist_iter_t where, void *data)
{
	node_t *new_node = NULL;
	
	assert(NULL != where);
	
	new_node = (node_t*)malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		/* find end of list iterator and return it */
		where = EndOfList(where);
		
		return (where);
	}
	
	new_node->data = data;
	PointNodeToNext(new_node, where);
	PointNodeToPrev(new_node, where->prev);
	PointNodeToNext(where->prev, new_node);
	PointNodeToPrev(where, new_node);
	
	return (new_node);
}

dlist_iter_t DListRemove(dlist_iter_t current)
{	
	dlist_iter_t next_backup = NULL;
	
	assert(NULL != current);
	
	next_backup = current->next;
	
	PointNodeToPrev(current->next, current->prev);
	PointNodeToNext(current->prev, current->next);

	free(current);

	return (next_backup);
}

dlist_iter_t DListBegin(const dlist_t *list)
{
	assert(NULL != list);
	
	return (list->head.next);
}

int DListIsEqual(dlist_iter_t iter1 , dlist_iter_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2);
}

dlist_iter_t DListNext(dlist_iter_t current)
{
	assert(NULL != current);
	
	return (current->next);
}

dlist_iter_t DListPrev(dlist_iter_t current)
{
	assert(NULL != current);
	
	return(current->prev);
}

void *DListGetData(dlist_iter_t current)
{
	assert(NULL != current);
	
	return (current->data);
}

void DListSetData(dlist_iter_t current, void *data)
{
	assert(NULL != current);
	
	current->data = data;
}

int DListIsEmpty(const dlist_t *list)
{
	assert(NULL != list);
	
	return (DListBegin(list) == DListEnd(list));
}

size_t DListSize(const dlist_t *list)
{
	size_t size = 0;
	
	node_t *runner = NULL;
	
	assert(NULL != list);
	
	runner = DListBegin(list);
	
	while(NULL != runner->next)
	{
		++size;
		
		runner = runner->next;
	}
	
	return (size);
}

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, int (*match_func)(void *data, const void *param), const void *param)
{
	node_t *runner = NULL;
	int found = FALSE;

	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != match_func);
	
	runner = from;
	
	while(to != runner)
	{
		found = match_func(runner->data, param);
		if(TRUE == found)
		{
			return (runner);
		}
		
		runner = runner->next;
	}
	
	return (to);
}

dlist_iter_t DListEnd(const dlist_t *list)
{
	assert(NULL != list);
	
	return ((dlist_iter_t)(&list->tail));
}

int DListForEach(dlist_iter_t from, dlist_iter_t to, int(*action_func)(void *data, void *param), void *action_param)
{
	node_t *runner = NULL;
	int status = SUCCESS;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action_func);
	
	runner = from;
	
	/* traverse list up to "to" node or until action function fails */
	while(to != runner)
	{
		status = action_func(runner->data, action_param);
		if(SUCCESS != status)
		{
			return (status);
		}
		
		runner = runner->next;
	}
	
	return (status);
}

dlist_iter_t DListPushFront(dlist_t *list, void *data)
{
	assert(NULL != list);

	/* insert node after head and return its iterator */
	return (DListInsert(DListBegin(list), data));
}

dlist_iter_t DListPushBack(dlist_t *list, void *data)
{
	assert(NULL != list);

	/* insert node before tail and return its iterator */
	return (DListInsert(DListEnd(list), data));
}

void *DListPopBack(dlist_t *list)
{
	node_t *node_to_remove = NULL;
	void *data_of_removed_node = NULL;
	
	assert(NULL != list);
	
	node_to_remove = DListPrev(DListEnd(list));
	data_of_removed_node = DListGetData(node_to_remove);
	
	DListRemove(node_to_remove);
	
	return (data_of_removed_node);
}

void *DListPopFront(dlist_t *list)
{
	node_t *node_to_remove = NULL;
	void *data_of_removed_node = NULL;
	
	assert(NULL != list);
	
	node_to_remove = DListBegin(list);
	data_of_removed_node = DListGetData(node_to_remove);
	
	DListRemove(node_to_remove);
	
	return (data_of_removed_node);
}

static node_t *EndOfList(node_t *runner)
{
	while(NULL != runner->next)
	{
		runner = runner->next;
	}
	
	return (runner);
}

static void ClearList(dlist_t *list_to_clear)
{
	size_t nodes_to_remove = DListSize(list_to_clear);
	
	while(nodes_to_remove)
	{
		DListPopBack(list_to_clear);
		
		--nodes_to_remove;
	}
}

int DListMultiFind(dlist_iter_t from, dlist_iter_t to, int (*match_func)(void *data, const void *param), const void *param, dlist_t *output_list)
{
	node_t *runner = NULL;
	node_t *status = NULL;
	
	int found = FALSE;

	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != match_func);
	
	runner = from;
	
	while(to != runner)
	{
		found = match_func(runner->data, param);
		if(TRUE == found)
		{
			status = DListPushBack(output_list, runner->data); 
			if(DListEnd(output_list) == status)
			{
				/* malloc failed */
				ClearList(output_list);
				
				return (FAILURE);
			}
		}
		
		runner = runner->next;
	}
	
	return (found);
}

static void PointNodeToNext(node_t *curr_node, node_t *next_node)
{
	curr_node->next = next_node;
}

static void PointNodeToPrev(node_t *curr_node, node_t *prev_node)
{
	curr_node->prev = prev_node;
}

dlist_iter_t DListSplice(dlist_iter_t where, dlist_iter_t from, dlist_iter_t to)
{
	node_t *last_to_insert = NULL;
	
	assert(NULL != where);
	assert(NULL != from);
	assert(NULL != to);
	
	last_to_insert = to->prev;
	
	from->prev->next = to;
    to->prev = from->prev;

    where->prev->next = from;
    from->prev = where->prev;

    where->prev = last_to_insert;
    last_to_insert->next = where;

	return (from);
}
