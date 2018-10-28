#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* A function (and global) that can trigger malloc fails on demand. */
int mallocFail = 0;
void* myMalloc(size_t size) {
	if (mallocFail) { return NULL; }
	else { return malloc(size); }
}

int main()
{
	// store the result of calling functions
	int result;

	// a variable to store data popped from the list
	int data;

	// a pointer to our doubly-linked list
	List *listPtr = NULL;

	// initialise the list - passing the address of our
	// list pointer as an input so it can be modified 
	// to point to the memory allocated for the list
	result = listInit(&listPtr);
	if (result != SUCCESS)
	{
		printf("An error occurred when attempting to initialise the list\n");
	}
	else
	{
		printf("List initialised successfully\n");
	}

	// try getting data at the current position
	// this should fail because the list is empty
	result = getDataAtCurr(listPtr, &data);
	if (result != GET_FROM_EMPTY_LIST)
	{
		printf("Error: should have detected attempt to get data from an empty list\n");
	}
	else
	{
		printf("Correctly identified attempt to get data from an empty list\n");
	}

	// try getting removing data at the current position
	// this should fail because the list is empty
	result = removeAtCurr(listPtr, &data, 1);
	if (result != REMOVE_FROM_EMPTY_LIST)
	{
		printf("Error: should have detected attempt to remove data from an empty list\n");
	}
	else
	{
		printf("Correctly identified attempt to remove data from an empty list\n");
	}

	// try  moving 'curr' to the next node in the list
	// this should fail because the list is empty
	result = currToNext(listPtr);
	if (result != MOVE_IN_EMPTY_LIST)
	{
		printf("Error: should have detected attempt to move position of 'curr' when list is empty\n");
	}
	else
	{
		printf("Correctly identified attempt to move position of 'curr' when list is empty\n");
	}

	// try  moving 'curr' to the previous node in the list
	// this should fail because the list is empty
	result = currToPrev(listPtr);
	if (result != MOVE_IN_EMPTY_LIST)
	{
		printf("Error: should have detected attempt to move position of 'curr' when list is empty\n");
	}
	else
	{
		printf("Correctly identified attempt to move position of 'curr' when list is empty\n");
	}

	// add a new item to the list
	int value = 20;
	result = insertAfterCurr(listPtr, value);
	if (result != SUCCESS)
	{
		printf("Error: failed to add new node to list\n");
	}
	else
	{
		printf("Inserting a new node into list successful\n");
	}

	// try  moving 'curr' to the next node in the list
	// this should fail because 'curr' is at the tail of the list, nothing after it
	result = currToNext(listPtr);
	if (result != MOVE_NEXT_AT_TAIL)
	{
		printf("Error: should have detected attempt to move 'curr' next when it is already at the tail of the list\n");
	}
	else
	{
		printf("Correctly identified attempt to move 'curr' next when it is already at the tail of the list\n");
	}

	// try  moving 'curr' to the previous node in the list
	// this should fail because 'curr' is at the head of the list, nothing before it
	result = currToPrev(listPtr);
	if (result != MOVE_PREV_AT_HEAD)
	{
		printf("Error: should have detected attempt to move 'curr' previous when it is already at the head of the list\n");
	}
	else
	{
		printf("Correctly identified attempt to move 'curr' previous when it is already at the head of the list\n");
	}

	// add another item to the list
	value = 30;
	result = insertAfterCurr(listPtr, value);
	if (result != SUCCESS)
	{
		printf("Error: failed to add node 2 to list\n");
	}
	else
	{
		printf("Inserting node 2 into list successful\n");
	}

	// try getting data at the current position
	// We should get the first value entered (20)
	result = getDataAtCurr(listPtr, &data);
	if (result != SUCCESS)
	{
		printf("Error: failed to get data from list\n");
	}
	if (data != 20)
	{
		printf("Error: data obtained from list should be %d but is %d instead\n", 20, data);
	}
	else
	{
		printf("Successfully managed to get data from the list\n");
	}

	// try  moving 'curr' to the next node in the list
	result = currToNext(listPtr);
	if (result != SUCCESS)
	{
		printf("Error: failed to move 'curr' to next node in list\n");
	}
	else
	{
		printf("Successfully managed to move 'curr' to next node in list\n");
	}

	// try removing a node
	result = removeAtCurr(listPtr, &data, 1);
	if (result != SUCCESS)		
	{
		printf("Error: failed to remove node from list\n");
	}
	else
	{
		printf("Successfully managed to remove node from list\n");
	}

	// try emptying the list
	result = listFree(listPtr);
	if (result != SUCCESS)
	{
		printf("Error: failed to free list\n");
	}
	else
	{
		printf("Successfully free-d the list\n");
	}

	return 0;
}