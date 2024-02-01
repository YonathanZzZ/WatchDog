#include <assert.h> /* assert */	
#include <stdlib.h> /* malloc, free */

#include "sortlist.h"

struct SortList
{
	dlist_t *list;
	compare_t compare;
};

enum true_or_false
{
	FALSE = 0,
	TRUE = 1
};

typedef struct 
{	
	compare_t cmp_func;
	void *data;	
}is_to_insert_param_t;

static dlist_iter_t GetDListIter(sort_iter_t sort_iter)
{
	return (sort_iter.iter);
}

static sort_iter_t GetSortIterRls(dlist_iter_t dlist_iter)
{
	sort_iter_t sort_iter;
	sort_iter.iter = dlist_iter;
	
	return (sort_iter);
}

static sort_iter_t GetSortIter(dlist_iter_t dlist_iter, sort_list_t *list)
{
	sort_iter_t sort_iter;
	sort_iter.iter = dlist_iter;
	
	#ifndef NDEBUG
	sort_iter.list = list;
	#endif /* NDEBUG */
	
	return (sort_iter);
}

sort_list_t *SortListCreate(compare_t func)
{
	sort_list_t *sort_list = NULL;
	
	assert(func);
	
	sort_list = (sort_list_t*)malloc(sizeof(sort_list_t));
	if(NULL == sort_list)
	{
		return NULL;
	}
	
	sort_list->list = DListCreate();
	if(NULL == sort_list->list)
	{
		free(sort_list);
		sort_list = NULL;
		
		return (NULL);
	}
	
	sort_list->compare = func;
	
	return(sort_list);
}

void SortListDestroy(sort_list_t *list)
{
	assert(NULL != list);
	
	DListDestroy(list->list);
	list->list = NULL;
	
	free(list);
}

sort_iter_t SortListRemove(sort_iter_t current)
{
	dlist_iter_t dlist_iter = GetDListIter(current);

	assert(NULL != dlist_iter);
	
	#ifndef NDEBUG
	return (GetSortIter(DListRemove(dlist_iter), current.list));
	#endif
	
	return (GetSortIterRls(DListRemove(dlist_iter))); 
}


void *SortListGetData(sort_iter_t current)
{
	dlist_iter_t dlist_iter = GetDListIter(current);
	
	assert(NULL != dlist_iter);
	
	return(DListGetData(dlist_iter));
}

size_t SortListSize(const sort_list_t *list)
{
	assert(NULL != list);
	
	return (DListSize(list->list));
}

sort_iter_t SortListBegin(const sort_list_t *list)
{
	assert(NULL != list);
	
	return (GetSortIter(DListBegin(list->list), (sort_list_t *)list));
}

sort_iter_t SortListEnd(const sort_list_t *list)
{
	assert(NULL != list);	
	
	return (GetSortIter(DListEnd(list->list), (sort_list_t *)list));
}

sort_iter_t SortListNext(sort_iter_t current)
{
	dlist_iter_t dlist_iter = GetDListIter(current);
	
	assert(NULL != dlist_iter);
	
	#ifndef NDEBUG
	return (GetSortIter(DListNext(dlist_iter), current.list));
	#endif
	
	return (GetSortIterRls(DListNext(dlist_iter)));
}

sort_iter_t SortListPrev(sort_iter_t current)
{
	dlist_iter_t dlist_iter = GetDListIter(current);
	
	assert(NULL != dlist_iter);
	
	#ifndef NDEBUG
	return (GetSortIter(DListPrev(dlist_iter), current.list));
	#endif
	
	return (GetSortIterRls(DListPrev(dlist_iter)));
}

int SortListIsEmpty(sort_list_t *list)
{
	assert(NULL != list);
	
	return (DListIsEmpty(list->list));
}

void *SortListPopFront(sort_list_t *list)
{
	assert(NULL != list);
	
	return (DListPopFront(list->list));
}

void *SortListPopBack(sort_list_t *list)
{
	assert(NULL != list);
	
	return (DListPopBack(list->list));
}

int SortListForEach(sort_iter_t from, sort_iter_t to, action_t action, void *action_param)
{
	dlist_iter_t dlist_from = NULL;
	dlist_iter_t dlist_to = NULL;
	
	assert(NULL != action);
	assert(NULL != from.list);
	assert(NULL != to.list);
	#ifndef NDEBUG
	assert(from.list == to.list);
	#endif /* NDEBUG */
	
	dlist_from = GetDListIter(from);
	dlist_to = GetDListIter(to);
	return (DListForEach(dlist_from, dlist_to, action, action_param));
}

static dlist_iter_t FindFunc(dlist_iter_t from, dlist_iter_t to, compare_t compare, void *param)
{
	dlist_iter_t runner = from;
	
	while(to != runner)
	{
		if (0 == compare(DListGetData(runner), param))
		{
			return (runner);
		}
		
		runner = DListNext(runner);
	}
	
	return (runner);
}

sort_iter_t SortListFind(sort_iter_t from, sort_iter_t to, sort_list_t *list, void *param)
{
	dlist_iter_t dlist_from = NULL;
	dlist_iter_t dlist_to = NULL;
	dlist_iter_t found_iter = NULL;
	
	assert(NULL != list);
	assert(NULL != from.list);
	assert(NULL != to.list);
	#ifndef NDEBUG
	assert(from.list == to.list);
	#endif /* NDEBUG */
	
	dlist_from = GetDListIter(from);
	dlist_to = GetDListIter(to);
	
	found_iter = FindFunc(dlist_from, dlist_to, list->compare, param);
	
	return (GetSortIter(found_iter, list));
}

sort_iter_t SortListFindIf(sort_iter_t from, sort_iter_t to, is_match_t is_match, void *param)
{
	dlist_iter_t dlist_from = NULL;
	dlist_iter_t dlist_to = NULL;
	dlist_iter_t found_iter = NULL;
	
	assert(NULL != is_match);
	assert(NULL != from.list);
	assert(NULL != to.list);
	#ifndef NDEBUG
	assert(from.list == to.list);
	#endif /* NDEBUG */
	
	dlist_from = GetDListIter(from);
	dlist_to = GetDListIter(to);
	
	found_iter = DListFind(dlist_from, dlist_to, is_match, param);
	
	#ifndef NDEBUG
	return (GetSortIter(found_iter, from.list));
	#endif
	
	return (GetSortIterRls(found_iter));
}

int SortListIsEqual(sort_iter_t iter1 , sort_iter_t iter2)
{
	assert(NULL != iter1.iter);
	assert(NULL != iter2.iter);
	
	return (DListIsEqual(GetDListIter(iter1), GetDListIter(iter2)));
}

static dlist_iter_t WhereToInsert(sort_list_t *list, void *data)
{
	dlist_iter_t runner = DListBegin(list->list);
	
	int comp_val = 0;
	
	while (DListEnd(list->list) != runner)
	{
		comp_val = list->compare(DListGetData(runner), data);
		if(0 < comp_val)
		{
			return (runner);
		}
		
		runner = DListNext(runner);
	}
	
	return (runner);
}

static int IsToInsert(void *data, const void *param)
{
	compare_t cmp_func = ((is_to_insert_param_t *)param)->cmp_func;
	const void *param_data = ((is_to_insert_param_t *)param)->data;
	
	return (0 < cmp_func(data, param_data));
}

sort_iter_t SortListInsert(sort_list_t *list, void *data)
{
	sort_iter_t where_to_insert = {NULL};
	is_to_insert_param_t param = {NULL};
	
	assert(NULL != list);
	
	param.cmp_func = list->compare;
	param.data = data;
	
	where_to_insert = SortListFindIf(SortListBegin(list), SortListEnd(list), IsToInsert, &param);
	
	return (GetSortIter(DListInsert(GetDListIter(where_to_insert), data), list));
}

void SortListMerge(sort_list_t *dest, sort_list_t *src)
{
	dlist_iter_t from = NULL;
	dlist_iter_t to = NULL;
	dlist_iter_t where_to_insert = NULL;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	to = GetDListIter(SortListBegin(src));
	
	while(NULL != DListNext(to))
	{
		from = to;
		where_to_insert = WhereToInsert(dest, DListGetData(from));
		
		if(GetDListIter(SortListEnd(dest)) == where_to_insert)
		{
			to = GetDListIter(SortListEnd(src));
		}
		
		else
		{
			to = WhereToInsert(src, DListGetData(where_to_insert));
		}
		
		if(from == to)
		{
			to = DListNext(to);
		}
	
		DListSplice(where_to_insert, from, to);
		
	}
}
