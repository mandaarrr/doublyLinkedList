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

	if(((*listPtr)->head == NULL)&&((*listPtr)->tail == NULL)&&((*listPtr)->curr == NULL))
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
			return SUCCESS;
		}

	else if((listPtr->curr == listPtr->head) && (listPtr->curr->prev == NULL))
		{
			node->next = listPtr->curr;
			listPtr->curr->prev = node;
			listPtr->head = node;
			return SUCCESS;
		}

	else if ((listPtr->head != listPtr->curr) && (listPtr->curr->prev != NULL))
		{
			node->next = listPtr->curr;
			node->prev = listPtr->curr->prev;
			listPtr->curr->prev->next = node;
			listPtr->curr->prev = node;
			return SUCCESS;
		}


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
		{
			listPtr->head = listPtr->tail = listPtr->curr = node;
			return SUCCESS;
		}

	else if ((listPtr->curr == listPtr->tail)&&(listPtr->curr->next == NULL)) //check if element is at the tail
		{
			node->prev = listPtr->curr;
			listPtr->curr->next = node;
			listPtr->tail = node;
			return SUCCESS;
		}

	else
		{
			node->prev = listPtr->curr;
			node->next = listPtr->curr->next;
			listPtr->curr->next->prev = node;
			listPtr->curr->next = node;
			return SUCCESS;
		}
}

/* Move 'curr' one position back in the list
   (towards head and away from tail).
		listPtr must not be NULL */
int currToPrev(List* listPtr)
{}


/* Move 'curr' one position forward in the list
   (towards tail and away from head).
		listPtr must not be NULL */
int currToNext(List* listPtr)
{}


/* Get the data stored at the current position in the list ('curr')
  (but do not remove it from the list). This is a 'peek' method.
		listPtr must not be NULL
		pointer to data must not be NULL */
int getDataAtCurr(List* listPtr, int *data)
{}


/* Remove the node at the current position ('curr') from the List
   storing its data in the data pointer provided.
   'curr' should always be bounded by 'head' and 'tail'.
   Upon removal, 'curr' should move one item forward in the list (if moveForward != 0).
   Otherwise, 'curr' should move one item backward in the list (if moveForward == 0).
		listPtr must not be NULL
		pointer to data must not be NULL*/
int removeAtCurr(List* listPtr, int *data, int moveForward)
{}


/* Free all memory used by the List. 
		listPtr must not be NULL */
int listFree(List* listPtr){}


/* We define this function in 'list_tester.c'.
   Always use this function to request memory.
   Use it, but don't define it. */
void* myMalloc(size_t size);
