/*
*Name: Mandar Tamhane
*Matric Number: 170021792
*Module Code: AC21008
*/




#include<stdio.h>
#include<stdlib.h>
#include"list.h"

/* Initialize a new List and record its pointer into (*listPtr).
		listPtr must not be NULL
		(*listPtr) must be NULL */
int listInit(List **listPtr)
{
	if(listPtr==NULL)
		return NULL_PARAM;

	if((*listPtr)!=NULL)
		return BAD_INIT_PARAM;

	(*listPtr) = (List*)myMalloc(sizeof(List));

	if((*listPtr)==NULL)
		return OUT_OF_MEMORY;

	(*listPtr)->head = NULL;
	(*listPtr)->tail = NULL;
	(*listPtr)->curr = NULL;

	if(((*listPtr)->head == NULL)&&((*listPtr)->tail == NULL)&&((*listPtr)->curr == NULL)) //confirming check
		return SUCCESS;

	return SUCCESS;

}


/* Insert 'data' as a new node into the List immediately before
   the current position ('curr')
		listPtr must not be NULL */
int insertBeforeCurr(List* listPtr, int data)
{
	ListNode* node = myMalloc(sizeof(ListNode));

	if (node == NULL)
		return OUT_OF_MEMORY;	

	node->data = data;
	node->next = NULL;
	node->prev = NULL;

	if(listPtr==NULL)
		return NULL_PARAM;

	if((listPtr->head == NULL)&&(listPtr->tail == NULL)&&(listPtr->curr == NULL)) //checking empty list
		{
			listPtr->head = listPtr->tail = listPtr->curr = node;
		}

	else if((listPtr->curr == listPtr->head) && (listPtr->curr->prev == NULL)) //check if element is at head
		{
			node->next = listPtr->curr;
			listPtr->curr->prev = node;
			listPtr->head = node;
		}

	else if ((listPtr->head != listPtr->curr) && (listPtr->curr->prev != NULL)) //add element before current
		{
			node->next = listPtr->curr;
			node->prev = listPtr->curr->prev;
			listPtr->curr->prev->next = node;
			listPtr->curr->prev = node;
		}
	
	return SUCCESS;

}


/* Insert 'data' as a new node into the List immediately after 
   the current position ('curr'.)
		listPtr must not be NULL */
int insertAfterCurr(List* listPtr, int data)
{
	ListNode* node = myMalloc(sizeof(ListNode));

	if (node == NULL)
		return OUT_OF_MEMORY;	

	node->data = data;
	node->next = NULL;
	node->prev = NULL;

	if(listPtr==NULL)
		return NULL_PARAM;

	if((listPtr->head == NULL)&&(listPtr->tail == NULL)&&(listPtr->curr == NULL)) //checking empty list
			listPtr->head = listPtr->tail = listPtr->curr = node;

	else if ((listPtr->curr == listPtr->tail)&&(listPtr->curr->next == NULL)) //check if element is at the tail
		{
			node->prev = listPtr->curr;
			listPtr->curr->next = node;
			listPtr->tail = node;
		}

	else if ((listPtr->tail != listPtr->curr)&&(listPtr->curr->next != NULL)) //entering after current
		{
			node->prev = listPtr->curr;
			node->next = listPtr->curr->next;
			listPtr->curr->next->prev = node;
			listPtr->curr->next = node;
		}

	return SUCCESS;
}

/* Move 'curr' one position back in the list
   (towards head and away from tail).
		listPtr must not be NULL */
int currToPrev(List* listPtr)
{
	if(listPtr == NULL)
		return NULL_PARAM;

	if(listPtr->head == NULL)
		return MOVE_IN_EMPTY_LIST;

	else if(listPtr->head == listPtr->curr) //trying to move current before head
		return MOVE_PREV_AT_HEAD;

	else
		{	
			listPtr->curr = listPtr->curr->prev;
			return SUCCESS;
		}
}


/* Move 'curr' one position forward in the list
   (towards tail and away from head).
		listPtr must not be NULL */
int currToNext(List* listPtr)
{
	if(listPtr == NULL)
		return NULL_PARAM;

	if(listPtr->head == NULL)
		return MOVE_IN_EMPTY_LIST;

	else if(listPtr->tail == listPtr->curr) //trying to move current after tail
		return MOVE_NEXT_AT_TAIL;

	else
		{	
			listPtr->curr = listPtr->curr->next;
			return SUCCESS;
		}
}


/* Get the data stored at the current position in the list ('curr')
  (but do not remove it from the list). This is a 'peek' method.
		listPtr must not be NULL
		pointer to data must not be NULL */
int getDataAtCurr(List* listPtr, int *data)
{
	if(listPtr == NULL)
		return NULL_PARAM;

	if(data == NULL)
		return NULL_PARAM;

	if(listPtr->head == NULL)
		return GET_FROM_EMPTY_LIST;

	else
		{
			*data = listPtr->curr->data;
			return SUCCESS;
		}
}


/* Remove the node at the current position ('curr') from the List
   storing its data in the data pointer provided.
   'curr' should always be bounded by 'head' and 'tail'.
   Upon removal, 'curr' should move one item forward in the list (if moveForward != 0).
   Otherwise, 'curr' should move one item backward in the list (if moveForward == 0).
		listPtr must not be NULL
		pointer to data must not be NULL*/
int removeAtCurr(List* listPtr, int *data, int moveForward)
{
	if(listPtr == NULL)
		return NULL_PARAM;

	if(data == NULL)
		return NULL_PARAM;

	if(listPtr->head == NULL)
		return REMOVE_FROM_EMPTY_LIST;

	if(listPtr->head == listPtr->tail)
	{
		*data = listPtr->curr->data;
		free(listPtr->curr);
		listPtr->head = listPtr->tail = listPtr->curr = NULL;
	}

	else if (listPtr->head == listPtr->curr)
	{
		*data = listPtr->curr->data;
		listPtr->head = listPtr->curr->next;
		listPtr->head->prev = NULL;
		free(listPtr->curr);
		listPtr->curr = listPtr->head;
	}

	else if (listPtr->tail == listPtr->curr)
	{
		*data = listPtr->curr->data;
		listPtr->tail = listPtr->curr->prev;
		listPtr->tail->next = NULL;
		free(listPtr->curr);
		listPtr->curr = listPtr->tail;
	}

	else if ((listPtr->head != listPtr->curr)&&(listPtr->tail != listPtr->curr))
	{
		*data = listPtr->curr->data;
		ListNode* node = listPtr->curr;
		listPtr->curr->prev->next = listPtr->curr->next;
		listPtr->curr->next->prev = listPtr->curr->prev;
		if (moveForward != 0)
			listPtr->curr = listPtr->curr->next;
		else if (moveForward == 0)
			listPtr->curr = listPtr->curr->prev;
		free(node);
	}

	return SUCCESS;
}


/* Free all memory used by the List. 
		listPtr must not be NULL */
int listFree(List* listPtr)
{

	if(listPtr == NULL)
		return NULL_PARAM;

	ListNode* node = listPtr->head;

	while(node != NULL)
	{
		listPtr->head = node->next;
		free(node);
		node = listPtr->head;
	}

	return SUCCESS;
}


/* We define this function in 'list_tester.c'.
   Always use this function to request memory.
   Use it, but don't define it. */
void* myMalloc(size_t size);
