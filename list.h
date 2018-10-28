#include <stddef.h>

// error codes
#define SUCCESS					100	// successful completion of a function
#define OUT_OF_MEMORY			101 // failed to allocate memory
#define BAD_INIT_PARAM			102 // initialisation parameter invalid, e.g. expected NULL when it isn't
#define NULL_PARAM				103 // a NULL value has been received when non-NULL was expected
#define REMOVE_FROM_EMPTY_LIST	104 // attempt to remove an item from an empty list
#define GET_FROM_EMPTY_LIST		105 // attempting to get data from an empty list
#define MOVE_IN_EMPTY_LIST		106	// attempting to move the 'curr' position in an empty list
#define MOVE_PREV_AT_HEAD		107 // attempting to move 'curr' to the previous node in the list when it is already at the head
#define MOVE_NEXT_AT_TAIL		108 // attempting to move 'curr' to the next node in the list when it is already at the tail

// NOTE: the following error code is optional and may not be required. In some cases, depending on how you have structured
// your code with if..else statements you could conceivably detect a malformed list, e.g. where the head is NULL but the tail
// isn't. In which case, this error code can be used if you need to return. This error code does NOT feature in the tester
// program and is not used by the automated marking system either.
#define BAD_LIST				109 


/* typedef for the node struct used in List. */
typedef struct ListNode{
	int data;
	struct ListNode* prev;
	struct ListNode* next;
} ListNode;


/* typedef for the List struct itself. */
typedef struct List{
	ListNode* head;
	ListNode* tail;
	ListNode* curr;
} List;


/* Initialize a new List and record its pointer into (*listPtr).
		listPtr must not be NULL
		(*listPtr) must be NULL */
int listInit(List **listPtr);


/* Insert 'data' as a new node into the List immediately before
   the current position ('curr')
		listPtr must not be NULL */
int insertBeforeCurr(List* listPtr, int data);


/* Insert 'data' as a new node into the List immediately after 
   the current position ('curr'.)
		listPtr must not be NULL */
int insertAfterCurr(List* listPtr, int data);


/* Move 'curr' one position back in the list
   (towards head and away from tail).
		listPtr must not be NULL */
int currToPrev(List* listPtr);


/* Move 'curr' one position forward in the list
   (towards tail and away from head).
		listPtr must not be NULL */
int currToNext(List* listPtr);


/* Get the data stored at the current position in the list ('curr')
  (but do not remove it from the list). This is a 'peek' method.
		listPtr must not be NULL
		pointer to data must not be NULL */
int getDataAtCurr(List* listPtr, int *data);


/* Remove the node at the current position ('curr') from the List
   storing its data in the data pointer provided.
   'curr' should always be bounded by 'head' and 'tail'.
   Upon removal, 'curr' should move one item forward in the list (if moveForward != 0).
   Otherwise, 'curr' should move one item backward in the list (if moveForward == 0).
		listPtr must not be NULL
		pointer to data must not be NULL*/
int removeAtCurr(List* listPtr, int *data, int moveForward);


/* Free all memory used by the List. 
		listPtr must not be NULL */
int listFree(List* listPtr);


/* We define this function in 'list_tester.c'.
   Always use this function to request memory.
   Use it, but don't define it. */
void* myMalloc(size_t size);
