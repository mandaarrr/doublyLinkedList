#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define	WIN  0
#define	FAIL 9999

/* A function (and global) that can trigger malloc fails on demand. */
int mallocFail = 0;
void* myMalloc(size_t size) {
	if (mallocFail) { return NULL; }
	else { return malloc(size); }
}

int test_listInit();
int test_listInsertBeforeCurr();
int test_listInsertAfterCurr();
int test_listCurrToPrev();
int test_listCurrToNext();
int test_listGetCurr();
int test_listRemoveCurr();
int test_listFree();

int main() {
	if (test_listInit() != 0) {
		printf("Quitting testing: 'listInit' failed.\n");
		return 0;
	}
	if (test_listInsertBeforeCurr() != 0) {
		printf("Quitting testing: 'listInsertBeforeCurr' failed.\n");
		return 0;
	}
	if (test_listInsertAfterCurr() != 0) {
		printf("Quitting testing: 'listInsertAfterCurr' failed.\n");
		return 0;
	}
	if (test_listCurrToPrev() != 0) {
		printf("Quitting testing: 'listCurrToPrev' failed.\n");
		return 0;
	}
	if (test_listCurrToNext() != 0) {
		printf("Quitting testing: 'listCurrToNext' failed.\n");
		return 0;
	}
	if (test_listGetCurr() != 0) {
		printf("Quitting testing: 'listGetCurr' failed.\n");
		return 0;
	}
	if (test_listRemoveCurr() != 0) {
		printf("Quitting testing: 'listRemoveCurr' failed.\n");
		return 0;
	}
	if (test_listFree() != 0) {
		printf("Quitting testing: 'listFree' failed.\n");
		return 0;
	}
	return 0;
}

/* Test 'listInit'.
** This function has a memory leak (List is never free'd) but we
** haven't tested 'listFree' yet, so can't exactly use it here. */
int test_listInit() {
	printf("Testing listInit...\n");

	// test for NULL_PARAM check
	int rVal = listInit(NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'listPtr' is not NULL\n"); return FAIL; }

	// BAD_INIT_PARAM
	int thing = 71;
	List *listPtr = (List*) (&thing);
	rVal = listInit(&listPtr);
	if (rVal != BAD_INIT_PARAM) { printf("  failed: should make sure '(*listPtr)' is NULL\n"); return FAIL; }

	// OUT_OF_MEMORY
	mallocFail = 1;
	listPtr = NULL;
	rVal = listInit(&listPtr);
	if (rVal != OUT_OF_MEMORY) { printf("  failed: should always check return from malloc (myMalloc)\n"); return FAIL; }
	mallocFail = 0;

	// SUCCESS
	listPtr = NULL;
	rVal = listInit(&listPtr);
	if (listPtr->head != NULL) { printf("  failed: 'listPtr->headPtr' was not set to NULL\n"); return FAIL; }
	if (listPtr->tail != NULL) { printf("  failed: 'listPtr->tailPtr' was not set to NULL\n"); return FAIL; }
	if (listPtr->curr != NULL) { printf("  failed: 'listPtr->currPtr' was not set to NULL\n"); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed: did not return SUCCESS: %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'listInsertBeforeCurr'.
** This function has a memory leak (List is never free'd) but we
** haven't tested 'listFree' yet, so can't exactly use it here. */
int test_listInsertBeforeCurr() {
	printf("Testing listInsertBeforeCurr...\n");

	// initialize a new List to test on
	List* listPtr = NULL;
	int rVal = listInit(&listPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (listInit) failed: %d\n", rVal); return FAIL; }

	// NULL_PARAM 1 [should fail]
	int v1 = (0x0000000F & rand()) | (0x00000001);
	rVal = insertBeforeCurr(NULL, v1);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'listPtr' is not NULL\n"); return FAIL; }

	// OUT_OF_MEMORY
	mallocFail = 1;
	rVal = insertBeforeCurr(listPtr, v1);
	if (rVal != OUT_OF_MEMORY) { printf("  failed: should always check return from myMalloc (malloc)\n"); return FAIL; }
	mallocFail = 0;

	// insert a single item onto empty List
	rVal = insertBeforeCurr(listPtr, v1);	// insert v1 into empty List
	if (listPtr->head == NULL) { printf("  failed1: 'listPtr->headPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->tail == NULL) { printf("  failed1: 'listPtr->tailPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->curr == NULL) { printf("  failed1: 'listPtr->currPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->head != listPtr->tail) { printf("  failed1: 'listPtr->headPtr' should equal 'listPtr->tailPtr' after insert into empty List\n"); return FAIL; }
	if (listPtr->head != listPtr->curr) { printf("  failed1: 'listPtr->headPtr' should equal 'listPtr->currPtr' after insert into empty List\n"); return FAIL; }
	if (listPtr->curr != listPtr->tail) { printf("  failed1: 'listPtr->currPtr' should equal 'listPtr->tailPtr' after insert into empty List\n"); return FAIL; }
	if (listPtr->curr->prev != NULL) { printf("  failed1: 'listPtr->currPtr->prevPtr' should be NULL after insert into empty list\n"); return FAIL; }
	if (listPtr->curr->next != NULL) { printf("  failed1: 'listPtr->currPtr->nextPtr' should be NULL after insert into empty List\n"); return FAIL; }
	if (listPtr->curr->data != v1) { printf("  failed1: 'listPtr->currPtr->data' is %d but should be %d\n", listPtr->curr->data, v1); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed1: vListInsertBeforeCurr %d did not return SUCCESS: %d\n", v1, rVal); return FAIL; }

	// then insertBefore another one (into a single-item list)
	int v2 = (0x000000F0 & rand()) | (0x00000010);
	rVal = insertBeforeCurr(listPtr, v2);	// this is what we're testing now

	// see what we've got after inserting two values
	if (listPtr->head == NULL) { printf("  failed2: 'listPtr->headPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->tail == NULL) { printf("  failed2: 'listPtr->tailPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->curr == NULL) { printf("  failed2: 'listPtr->currPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->head == listPtr->tail) { printf("  failed2: 'listPtr->headPtr' should NOT equal 'listPtr->tailPtr' after insertBefore into non-empty List\n"); return FAIL; }
	if (listPtr->head == listPtr->curr) { printf("  failed2: 'listPtr->headPtr' should NOT equal 'listPtr->currPtr' after insertBefore into non-empty List\n"); return FAIL; }
	if (listPtr->curr != listPtr->tail) { printf("  failed2: 'listPtr->currPtr' should equal 'listPtr->tailPtr' after insertBefore into single-item List\n"); return FAIL; }
	if (listPtr->head->prev != NULL) { printf("  failed2: 'listPtr->headPtr->prevPtr' should be NULL after any insert\n"); return FAIL; }
	if (listPtr->head->next != listPtr->tail) { printf("  failed2: 'listPtr->headPtr->nextPtr' should equal 'listPtr->tailPtr' after insert into single-item List\n"); return FAIL; }
	if (listPtr->head->data != v2) { printf("  failed2: 'vListPtr->headPtr->data' is %d after but should be %d\n", listPtr->head->data, v2); return FAIL; }
	if (listPtr->tail->prev != listPtr->head) { printf("  failed2: 'listPtr->tailPtr->prevPtr' should equal 'listPtr->headPtr' after insert into single-item List\n"); return FAIL; }
	if (listPtr->tail->next != NULL) { printf("  failed2: 'listPtr->tailPtr->nextPtr' should equal NULL after any insert\n"); return FAIL; }
	if (listPtr->tail->data != v1) { printf("  failed2: 'listPtr->tailPtr->data' is %d, but should be %d\n", listPtr->tail->data, v1); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed2: listInsertBeforeCurr %d did not return SUCCESS: %d\n", v2, rVal); return FAIL; }

	// then insertBefore another one (into a two-item list)
	int v3 = (0x00000F00 & rand()) | (0x00000100);
	rVal = insertBeforeCurr(listPtr, v3);	// this is what we're testing now

	// see what we've got after inserting three values
	if (listPtr->head == NULL) { printf("  failed3: 'listPtr->headPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->tail == NULL) { printf("  failed3: 'listPtr->tailPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->curr == NULL) { printf("  failed3: 'listPtr->currPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->head == listPtr->tail) { printf("  failed3: 'listPtr->headPtr' should NOT equal 'listPtr->tailPtr' after insert into non-empty List\n"); return FAIL; }
	if (listPtr->head == listPtr->curr) { printf("  failed3: 'listPtr->headPtr' should NOT equal 'listPtr->currPtr' after insertBefore into non-empty List\n"); return FAIL; }
	if (listPtr->curr != listPtr->tail) { printf("  failed3: 'listPtr->currPtr' should equal 'listPtr->tailPtr' after insertBefore when curr==tail\n"); return FAIL; }
	if (listPtr->head->prev != NULL) { printf("  failed3: 'listPtr->headPtr->prevPtr' should be NULL after any insert\n"); return FAIL; }
	if (listPtr->head->data != v2) { printf("  failed3: 'listPtr->headPtr->data' is %d, but should be %d\n", listPtr->head->data, v2); return FAIL; }
	if (listPtr->tail->next != NULL) { printf("  failed3: 'listPtr->tailPtr->nextPtr' should equal NULL after insertBefore into single-item List\n"); return FAIL; }
	if (listPtr->tail->data != v1) { printf("  failed3: 'listPtr->tailPtr->data' is %d, but should be %d\n", listPtr->tail->data, v1); return FAIL; }
	if (listPtr->head->next == NULL) { printf("  failed3: 'listPtr->headPtr->nextPtr' should not be NULL after insert into two-item List\n"); return FAIL; }
	if (listPtr->tail->prev == NULL) { printf("  failed3: 'listPtr->tailPtr->prevPtr' should not be NULL after insert into two-item List\n"); return FAIL; }

	if (listPtr->head->next != listPtr->tail->prev) { printf("  failed3: 'listPtr->headPtr->nextPtr' should equal 'listPtr->tailPtr->prevPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->head->next == listPtr->head) { printf("  failed3: 'listPtr->headPtr->nextPtr' should NOT equal 'listPtr->headPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->head->next == listPtr->tail) { printf("  failed3: 'listPtr->headPtr->nextPtr' should NOT equal 'listPtr->tailPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->tail->prev == listPtr->tail) { printf("  failed3: 'listPtr->tailPtr->prevPtr' should NOT equal 'listPtr->tailPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->tail->prev == listPtr->head) { printf("  failed3: 'listPtr->tailPtr->prevPtr' should NOT equal 'listPtr->headPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->head->next->next != listPtr->tail) { printf("  failed3: 'listPtr->headPtr->nextPtr->nextPtr' should equal 'vListPtr->tailPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->head->next->prev != listPtr->head) { printf("  failed3: 'listPtr->headPtr->nextPtr->prevPtr' should equal 'vListPtr->headPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->tail->prev->prev != listPtr->head) { printf("  failed3: 'listPtr->tailPtr->prevPtr->prevPtr' should equal 'vListPtr->headPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->tail->prev->next != listPtr->tail) { printf("  failed3: 'listPtr->tailPtr->prevPtr->nextPtr' should equal 'vListPtr->tailPtr' after insertBefore into two-item List when curr==tail\n"); return FAIL; }
	if (listPtr->head->next->data != v3) { printf("  failed3: 'listPtr->headPtr->nextPtr->data' is %d, but should be %d\n", listPtr->head->next->data, v3); return FAIL; }
	if (listPtr->tail->prev->data != v3) { printf("  failed3: 'listPtr->tailPtr->prevPtr->data' is %d, but should be %d\n", listPtr->tail->prev->data, v3); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed3: listInsertBeforeCurr %d did not return SUCCESS: %d\n", v3, rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'listInsertAfterCurr'.
** This function has a memory leak (List is never free'd) but we
** haven't tested 'listFree' yet, so can't exactly use it here. */
int test_listInsertAfterCurr() {
	printf("Testing listInsertAfterCurr...\n");

	// initialize a new List to test on
	List* listPtr = NULL;
	int rVal = listInit(&listPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (listInit) failed: %d\n", rVal); return FAIL; }

	// NULL_PARAM 1 [should fail]
	int v1 = (0x0000000F & rand()) | (0x00000001);
	rVal = insertAfterCurr(NULL, v1);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'listPtr' is not NULL\n"); return FAIL; }

	// OUT_OF_MEMORY
	mallocFail = 1;
	rVal = insertAfterCurr(listPtr, v1);
	if (rVal != OUT_OF_MEMORY) { printf("  failed: should always check return from myMalloc (malloc)\n"); return FAIL; }
	mallocFail = 0;

	// insert a single item onto empty List
	rVal = insertAfterCurr(listPtr, v1);	// insert v1 into empty List
	if (listPtr->head == NULL) { printf("  failed1: 'listPtr->headPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->tail == NULL) { printf("  failed1: 'listPtr->tailPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->curr == NULL) { printf("  failed1: 'listPtr->currPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->head != listPtr->tail) { printf("  failed1: 'listPtr->headPtr' should equal 'listPtr->tailPtr' after insert into empty List\n"); return FAIL; }
	if (listPtr->head != listPtr->curr) { printf("  failed1: 'listPtr->headPtr' should equal 'listPtr->currPtr' after insert into empty List\n"); return FAIL; }
	if (listPtr->curr != listPtr->tail) { printf("  failed1: 'listPtr->currPtr' should equal 'listPtr->tailPtr' after insert into empty List\n"); return FAIL; }
	if (listPtr->curr->prev != NULL) { printf("  failed1: 'listPtr->currPtr->prevPtr' should be NULL after insert into empty list\n"); return FAIL; }
	if (listPtr->curr->next != NULL) { printf("  failed1: 'listPtr->currPtr->nextPtr' should be NULL after insert into empty List\n"); return FAIL; }
	if (listPtr->curr->data != v1) { printf("  failed1: 'listPtr->currPtr->data' is %d but should be %d\n", listPtr->curr->data, v1); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed1: listInsertAfterCurr %d did not return SUCCESS: %d\n", v1, rVal); return FAIL; }

	// then insertBefore another one (into a single-item list)
	int v2 = (0x000000F0 & rand()) | (0x00000010);
	rVal = insertAfterCurr(listPtr, v2);	// this is what we're testing now

	// see what we've got after inserting two values
	if (listPtr->head == NULL) { printf("  failed2: 'listPtr->headPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->tail == NULL) { printf("  failed2: 'listPtr->tailPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->curr == NULL) { printf("  failed2: 'listPtr->currPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->head == listPtr->tail) { printf("  failed2: 'listPtr->headPtr' should NOT equal 'listPtr->tailPtr' after insert into non-empty List\n"); return FAIL; }
	if (listPtr->head != listPtr->curr) { printf("  failed2: 'listPtr->headPtr' should equal 'listPtr->currPtr' after insertAfter into non-empty List\n"); return FAIL; }
	if (listPtr->curr == listPtr->tail) { printf("  failed2: 'listPtr->currPtr' should NOT equal 'listPtr->tailPtr' after insertAfter into single-item List\n"); return FAIL; }
	if (listPtr->head->prev != NULL) { printf("  failed2: 'listPtr->headPtr->prevPtr' should be NULL after any insert\n"); return FAIL; }
	if (listPtr->head->next != listPtr->tail) { printf("  failed2: 'listPtr->headPtr->nextPtr' should equal 'listPtr->tailPtr' after insert into single-item List\n"); return FAIL; }
	if (listPtr->head->data != v1) { printf("  failed2: 'listPtr->headPtr->data' is %d but should be %d after insertAfter into single-item List\n", listPtr->head->data, v1); return FAIL; }
	if (listPtr->tail->prev != listPtr->head) { printf("  failed2: 'listPtr->tailPtr->prevPtr' should equal 'listPtr->headPtr' after insert into single-item List\n"); return FAIL; }
	if (listPtr->tail->next != NULL) { printf("  failed2: 'listPtr->tailPtr->nextPtr' should equal NULL after any insert\n"); return FAIL; }
	if (listPtr->tail->data != v2) { printf("  failed2: 'listPtr->tailPtr->data' is %d, but should be %d after insertBefore into single-item List\n", listPtr->tail->data, v2); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed2: listInsertAfterCurr %d did not return SUCCESS: %d\n", v2, rVal); return FAIL; }

	// then insertAfter another one (into a two-item list)
	int v3 = (0x00000F00 & rand()) | (0x00000100);
	rVal = insertAfterCurr(listPtr, v3);	// this is what we're testing now

	// see what we've got after inserting three values
	if (listPtr->head == NULL) { printf("  failed3: 'listPtr->headPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->tail == NULL) { printf("  failed3: 'listPtr->tailPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->curr == NULL) { printf("  failed3: 'listPtr->currPtr' should never be NULL after insert\n"); return FAIL; }
	if (listPtr->head == listPtr->tail) { printf("  failed3: 'listPtr->headPtr' should NOT equal 'listPtr->tailPtr' after insert into non-empty List\n"); return FAIL; }
	if (listPtr->head != listPtr->curr) { printf("  failed3: 'listPtr->headPtr' should equal 'listPtr->currPtr' after insertAfter into non-empty List\n"); return FAIL; }
	if (listPtr->curr == listPtr->tail) { printf("  failed3: 'listPtr->currPtr' should NOT equal 'listPtr->tailPtr' after insertAfter when curr==head\n"); return FAIL; }
	if (listPtr->head->prev != NULL) { printf("  failed3: 'listPtr->headPtr->prevPtr' should be NULL after any insert\n"); return FAIL; }
	if (listPtr->head->data != v1) { printf("  failed3: 'listPtr->headPtr->data' is %d, but should be %d\n", listPtr->head->data, v1); return FAIL; }
	if (listPtr->tail->next != NULL) { printf("  failed3: 'listPtr->tailPtr->nextPtr' should equal NULL after any insert\n"); return FAIL; }
	if (listPtr->tail->data != v2) { printf("  failed3: 'listPtr->tailPtr->data' is %d, but should be %d\n", listPtr->tail->data, v2); return FAIL; }
	if (listPtr->head->next == NULL) { printf("  failed3: 'listPtr->headPtr->nextPtr' should not be NULL after insert into two-item List\n"); return FAIL; }
	if (listPtr->tail->prev == NULL) { printf("  failed3: 'listPtr->tailPtr->prevPtr' should not be NULL after insert into two-item List\n"); return FAIL; }
	if (listPtr->head->next != listPtr->tail->prev) { printf("  failed3: 'listPtr->headPtr->nextPtr' should equal 'listPtr->tailPtr->prevPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->head->next == listPtr->head) { printf("  failed3: 'listPtr->headPtr->nextPtr' should NOT equal 'listPtr->headPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->head->next == listPtr->tail) { printf("  failed3: 'listPtr->headPtr->nextPtr' should NOT equal 'listPtr->tailPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->tail->prev == listPtr->tail) { printf("  failed3: 'listPtr->tailPtr->prevPtr' should NOT equal 'listPtr->tailPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->tail->prev == listPtr->head) { printf("  failed3: 'listPtr->tailPtr->prevPtr' should NOT equal 'listPtr->headPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->head->next->next != listPtr->tail) { printf("  failed3: 'listPtr->headPtr->nextPtr->nextPtr' should equal 'listPtr->tailPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->head->next->prev != listPtr->head) { printf("  failed3: 'listPtr->headPtr->nextPtr->prevPtr' should equal 'listPtr->headPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->tail->prev->prev != listPtr->head) { printf("  failed3: 'listPtr->tailPtr->prevPtr->prevPtr' should equal 'listPtr->headPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->tail->prev->next != listPtr->tail) { printf("  failed3: 'listPtr->tailPtr->prevPtr->nextPtr' should equal 'listPtr->tailPtr' after insertAfter into two-item List when curr==head\n"); return FAIL; }
	if (listPtr->head->next->data != v3) { printf("  failed3: 'listPtr->headPtr->nextPtr->data' is %d, but should be %d in two-item List when curr==head\n", listPtr->head->next->data, v3); return FAIL; }
	if (listPtr->tail->prev->data != v3) { printf("  failed3: 'listPtr->tailPtr->prevPtr->data' is %d, but should be %d in two-item List when curr==head\n", listPtr->tail->prev->data, v3); return FAIL; }
	if (rVal != SUCCESS) { printf("  failed3: innsertAfterCurr %d did not return SUCCESS: %d\n", v3, rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'listCurrToPrev'.
** This function has a memory leak (List is never free'd) but we
** haven't tested 'listFree' yet, so can't exactly use it here. */
int test_listCurrToPrev() {
	printf("Testing listCurrToPrev...\n");

	// test for NULL_PARAM check
	int rVal = currToPrev(NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'listPtr' is not NULL\n"); return FAIL; }

	// initialize a vList to test on
	List* listPtr = NULL;
	rVal = listInit(&listPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (listInit) failed: %d\n", rVal); return FAIL; }

	// test for MOVE_IN_EMPTY_LIST check
	rVal = currToPrev(listPtr);
	if (rVal != MOVE_IN_EMPTY_LIST) { printf("  failed: should make sure List is not empty before moving curr\n"); return FAIL; }

	// insert item 1
	int v1 = 10;
	rVal = insertBeforeCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert 1 (insertBeforeCurr) failed: %d\n", rVal); return FAIL; }

	// now at head, so should return MOVE_PREV_AT_HEAD
	rVal = currToPrev(listPtr);
	if (rVal != MOVE_PREV_AT_HEAD) { printf("  failed: should check if 'curr==head' before moving\n"); return FAIL; }

	// insert item 2
	rVal = insertBeforeCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert 2 (insertBeforeCurr) failed: %d\n", rVal); return FAIL; }
	// insert item 3
	rVal = insertBeforeCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert 3 (insertBeforeCurr) failed: %d\n", rVal); return FAIL; }

	// should be able to move to prev twice before curr==head
	rVal = currToPrev(listPtr);
	if (rVal != SUCCESS) { printf("  failed: should be able to move curr one back in a three-item list when curr==tail\n"); return FAIL; }
	rVal = currToPrev(listPtr);
	if (rVal != SUCCESS) { printf("  failed: should be able to move curr one back in a three-item list when curr is one before tail\n"); return FAIL; }

	// now at head, so should return MOVE_PREV_AT_HEAD
	rVal = currToPrev(listPtr);
	if (rVal != MOVE_PREV_AT_HEAD) { printf("  failed: should check if 'curr==head' before moving\n"); return FAIL; }
	else /* if (rVal == MOVE_PREV_AT_HEAD) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'listCurrToNext'.
** This function has a memory leak (List is never free'd) but we
** haven't tested 'listFree' yet, so can't exactly use it here. */
int test_listCurrToNext() {
	printf("Testing listCurrToNext...\n");

	// test for NULL_PARAM check
	int rVal = currToNext(NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'listPtr' is not NULL\n"); return FAIL; }

	// initialize a vList to test on
	List* listPtr = NULL;
	rVal = listInit(&listPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (listInit) failed: %d\n", rVal); return FAIL; }

	// test for MOVE_IN_EMPTY_LIST check
	rVal = currToNext(listPtr);
	if (rVal != MOVE_IN_EMPTY_LIST) { printf("  failed: should make sure List is not empty before moving curr\n"); return FAIL; }

	// insert item 1 to make a 1 item list where head == curr == tail
	int v1 =10;
	rVal = insertAfterCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert 1 (insertAfterCurr) failed: %d\n", rVal); return FAIL; }

	// now at tail, so should return MOVE_NEXT_AT_TAIL
	rVal = currToNext(listPtr);
	if (rVal != MOVE_NEXT_AT_TAIL) { printf("  failed: should check if 'curr==tail' before moving\n"); return FAIL; }

	// insert item 2
	rVal = insertAfterCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert 2 (insertAfterCurr) failed: %d\n", rVal); return FAIL; }
	// insert item 3
	rVal = insertAfterCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert 3 (insertAfterCurr) failed: %d\n", rVal); return FAIL; }

	// should be able to move to next twice before curr==tail
	rVal = currToNext(listPtr);
	if (rVal != SUCCESS) { printf("  failed: should be able to move curr one forward in a three-item list when curr==head\n"); return FAIL; }
	rVal = currToNext(listPtr);
	if (rVal != SUCCESS) { printf("  failed: should be able to move curr one forward in a three-item list when curr one after head\n"); return FAIL; }

	// now at tail, so should return MOVE_NEXT_AT_TAIL
	rVal = currToNext(listPtr);
	if (rVal != MOVE_NEXT_AT_TAIL) { printf("  failed: should check if 'curr==tail' before moving\n"); return FAIL; }
	else /* if (rVal == MOVE_NEXT_AT_TAIL) */ { printf("  succeeded\n"); return WIN; }
}

/* Test 'listGetCurr'.
** This function has a memory leak (List is never free'd) but we
** haven't tested 'listFree' yet, so can't exactly use it here. */
int test_listGetCurr() {
	printf("Testing listGetCurr...\n");

	// initialize a vList to test on
	List* listPtr = NULL;
	int rVal = listInit(&listPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (listInit) failed: %d\n", rVal); return FAIL; }

	// test NULL_PARAMs
	int data;
	rVal = getDataAtCurr(NULL, &data);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'listPtr' is not NULL\n"); return FAIL; }
	rVal = getDataAtCurr(listPtr, NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'data' pointer is not NULL\n"); return FAIL; }

	// test get from empty list
	rVal = getDataAtCurr(listPtr, &data);
	if (rVal != GET_FROM_EMPTY_LIST) { printf("  failed: should check for empty list before getting curr\n"); return FAIL; }

	// insert item 1 and get it
	int v1 = 17;
	rVal = insertAfterCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert 1 (insertAfterCurr) failed: %d\n", rVal); return FAIL; }
	rVal = getDataAtCurr(listPtr, &data);
	if (rVal != SUCCESS) { printf("  failed: getDataAtCurr gave %d when %d expected. rVal = %d\n", data, v1, rVal); return FAIL; }

	// insert item 2 and get it
	int v2 = 27;
	rVal = insertAfterCurr(listPtr, v2);
	if (rVal != SUCCESS) { printf("  insert 2 (insertAfterCurr) failed: %d\n", rVal); return FAIL; }
	rVal = getDataAtCurr(listPtr, &data);
	if (rVal != SUCCESS) { printf("  failed: getDataAtCurr gave %d when %d expected. rVal = %d\n", data, v1, rVal); return FAIL; }
	rVal = currToNext(listPtr);
	if (rVal != SUCCESS) { printf("  move next 1 (currToNext) failed: %d\n", rVal); return FAIL; }
	rVal = getDataAtCurr(listPtr, &data);
	if (rVal != SUCCESS) { printf("  failed: getDataAtCurr gave %d when %d expected. rVal = %d\n", data, v2, rVal); return FAIL; }
	else /* if (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}


/* Test 'listRemoveCurr'.
** This function has a memory leak (List is never free'd) but we
** haven't tested 'listFree' yet, so can't exactly use it here. */
int test_listRemoveCurr() {
	printf("Testing listRemoveCurr...\n");

	// initialize a vList to test on
	List* listPtr = NULL;
	int rVal = listInit(&listPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (listInit) failed: %d\n", rVal); return FAIL; }

	// test NULL_PARAM & BAD_PARAM
	int moveForward = 1;
	int moveBackward = 0;
	int data;
	rVal = removeAtCurr(NULL, &data, moveForward);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'listPtr' is not NULL\n"); return FAIL; }
	rVal = removeAtCurr(listPtr, NULL, moveForward);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'data' pointer is not NULL\n"); return FAIL; }

	// test remove from empty list
	rVal = removeAtCurr(listPtr, &data, moveForward);
	if (rVal != REMOVE_FROM_EMPTY_LIST) { printf("  failed: should check for empty list before removing curr\n"); return FAIL; }

	// add 5 elements
	for (int i=10; i<=50; i=i+10) {
		rVal = insertAfterCurr(listPtr, i);
		if (rVal != SUCCESS) { printf("  insert %d (insertAfterCurr) failed: %d\n", i, rVal); return FAIL; }
	}

	// ensure curr is at the head of the list (although it already should be)
	while (currToPrev(listPtr) == SUCCESS);	

	// remove 1 node - leaving 4
	rVal = removeAtCurr(listPtr, &data, moveForward);	// remove the first (head) element
	if (rVal != SUCCESS) { printf("  failed: when 'curr=head' in five-item list. rVal=%d\n", rVal); return FAIL; }

	// 4 left: check that curr and head are updated properly
	if (listPtr->head != listPtr->curr) { printf("  failed: 'listPtr->headPtr' should equal 'listPtr->currPtr' after removal when 'curr=head' in five-item list\n"); return FAIL; }
	if (listPtr->head->next->next->next != listPtr->tail) { printf("  failed: 'listPtr->headPtr->nextPtr->nextPtr->nextPtr' should equal 'listPtr->tailPtr' after removal when 'curr=head' in five-item list\n"); return FAIL; }
	if (listPtr->tail->prev->prev->prev != listPtr->head) { printf("  failed: 'listPtr->tailPtr->prevPtr->prevPtr->prevPtr' should equal 'listPtr->headPtr' after removal when 'curr=head' in five-item list\n"); return FAIL; }

	// set curr to tail and then remove a node to make 3 left
	while (currToNext(listPtr) == SUCCESS);	// set curr to tail
	rVal = removeAtCurr(listPtr, &data, moveForward);	// remove the last (tail) element
	if (rVal != SUCCESS) { printf("  failed: when 'curr=tail' in four-item list. rVal=%d\n", rVal); return FAIL; }

	// 3 left: check that curr and tail are updated properly
	if (listPtr->tail != listPtr->curr) { printf("  failed: 'listPtr->tailPtr' should equal 'listPtr->currPtr' after removal when 'curr=tail' in four-item list\n"); return FAIL; }
	if (listPtr->head->next->next != listPtr->tail) { printf("  failed: 'listPtr->headPtr->nextPtr->nextPtr' should equal 'listPtr->tailPtr' after removal when 'curr=tail' in four-item list\n"); return FAIL; }
	if (listPtr->tail->prev->prev != listPtr->head) { printf("  failed: 'listPtr->tailPtr->prevPtr->prevPtr' should equal 'listPtr->headPtr' after removal when 'curr=tail' in four-item list\n"); return FAIL; }

	// set curr to middle and remove (with forward) -> 2 left
	rVal = currToPrev(listPtr);
	if (rVal != SUCCESS) { printf("  move back 1 (currToPrev) failed: %d\n", rVal); return FAIL; }
	rVal = removeAtCurr(listPtr, &data, moveForward);	// remove the middle element
	if (rVal != SUCCESS) { printf("  failed: when 'curr' is middle (moveForward) in three-item list. rVal=%d\n", rVal); return FAIL; }

	// check that head, curr and tail are updated properly
	if (listPtr->tail != listPtr->curr) { printf("  failed: 'listPtr->tailPtr' should equal 'listPtr->currPtr' after removal (moving forward) when 'curr' is middle in three-item list\n"); return FAIL; }
	if (listPtr->head == listPtr->curr) { printf("  failed: 'listPtr->headPtr' should NOT equal 'listPtr->currPtr' after removal (moving forward) when 'curr' is middle in three-item list\n"); return FAIL; }
	if (listPtr->head->next != listPtr->tail) { printf("  failed: 'listPtr->headPtr->nextPtr' should equal 'listPtr->tailPtr' after removal when 'curr' is middle in three-item list\n"); return FAIL; }
	if (listPtr->tail->prev != listPtr->head) { printf("  failed: 'listPtr->tailPtr->prevPtr' should equal 'listPtr->headPtr' after removal when 'curr' is middle in three-item list\n"); return FAIL; }

	// add 1 element -> back to 3 elements
	int v1 = 60;
	rVal = insertBeforeCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert %d (insertBeforeCurr) failed: %d\n", v1, rVal); return FAIL; }

	// move curr to middle, then remove (with backward)
	// make sure curr is at the tail (it already should be)
	while (currToNext(listPtr) == SUCCESS);	
	//now move curr back 1 place to the middle
	rVal = currToPrev(listPtr);
	if (rVal != SUCCESS) { printf("  move back 2 (currToPrev) failed: %d\n", rVal); return FAIL; }
	rVal = removeAtCurr(listPtr, &data, moveBackward);	// remove the middle element
	if (rVal != SUCCESS) { printf("  failed: when 'curr' is middle (moveBackward) in three-item list. rVal=%d\n", rVal); return FAIL; }

	// check that head, curr and tail are updated properly
	if (listPtr->head != listPtr->curr) { printf("  failed: 'listPtr->headPtr' should equal 'listPtr->currPtr' after removal (moving backward) when 'curr' is middle in three-item list\n"); return FAIL; }
	if (listPtr->tail == listPtr->curr) { printf("  failed: 'listPtr->tailPtr' should NOT equal 'listPtr->currPtr' after removal (moving backward) when 'curr' is middle in three-item list\n"); return FAIL; }
	if (listPtr->head->next != listPtr->tail) { printf("  failed: 'listPtr->headPtr->nextPtr' should equal 'listPtr->tailPtr' after removal when 'curr' is middle in three-item list\n"); return FAIL; }
	if (listPtr->tail->prev != listPtr->head) { printf("  failed: 'listPtr->tailPtr->prevPtr' should equal 'listPtr->headPtr' after removal when 'curr' is middle in three-item list\n"); return FAIL; }

	// 2 left: set curr to head and remove -> 1 left
	while (currToPrev(listPtr) == SUCCESS);	// set curr to head
	rVal = removeAtCurr(listPtr, &data, moveForward);	// remove the first (head) element
	if (rVal != SUCCESS) { printf("  failed: when 'curr=head' in two-item list. rVal=%d\n", rVal); return FAIL; }

	// check head, curr, tail all same
	if (listPtr->head != listPtr->curr) { printf("  failed: 'listPtr->headPtr' should equal 'listPtr->currPtr' after removal when 'curr=head' in two-item list\n"); return FAIL; }
	if (listPtr->head != listPtr->tail) { printf("  failed: 'listPtr->headPtr' should equal 'listPtr->tailPtr' after removal when 'curr=head' in two-item list\n"); return FAIL; }
	if (listPtr->curr != listPtr->tail) { printf("  failed: 'listPtr->currPtr' should equal 'listPtr->tailPtr' after removal when 'curr=head' in two-item list\n"); return FAIL; }

	// 1 left: remove final node -> 0 left
	rVal = removeAtCurr(listPtr, &data, moveForward);	// remove the final element
	if (rVal != SUCCESS) { printf("  failed: for single-item list. rVal=%d\n", rVal); return FAIL; }
	// check head, curr, tail all NULL
	if (listPtr->head != NULL) { printf("  failed: 'listPtr->headPtr' should equal NULL after removal in single-item list\n"); return FAIL; }
	if (listPtr->curr != NULL) { printf("  failed: 'listPtr->currPtr' should equal NULL after removal in single-item list\n"); return FAIL; }
	if (listPtr->tail != NULL) { printf("  failed: 'listPtr->tailPtr' should equal NULL after removal in single-item list\n"); return FAIL; }

	// test remove from empty list again
	rVal = removeAtCurr(listPtr, &data, moveForward);
	if (rVal != REMOVE_FROM_EMPTY_LIST) { printf("  failed: should check for empty list before removing curr\n"); return FAIL; }
	else /* (rVal == REMOVE_FROM_EMPTY_LIST) */ { printf("  succeeded\n"); return WIN; }
}


/* Test 'listFree'.
   Weird tests because I can't verify that 'free' ran - the pointers remain... */
int test_listFree() {
	printf("Testing listFree...\n");

	// test for NULL_PARAM check
	int rVal = listFree(NULL);
	if (rVal != NULL_PARAM) { printf("  failed: should make sure 'listPtr' is not NULL\n"); return FAIL; }	

	// test freeing empty List + freeDatas
	List* listPtr = NULL;
	rVal = listInit(&listPtr);
	if (rVal != SUCCESS) { printf("  initialize 1 (listInit) failed: %d\n", rVal); return FAIL; }
	rVal = listFree(listPtr);
	if (rVal != SUCCESS) { printf("  failed 1 (empty List): with error %d\n", rVal); return FAIL; }

	// test freeing !empty List
	listPtr = NULL;
	rVal = listInit(&listPtr);
	if (rVal != SUCCESS) { printf("  initialize 2 (listInit) failed: %d\n", rVal); return FAIL; }
	int v1 = 10;
	rVal = insertBeforeCurr(listPtr, v1);
	if (rVal != SUCCESS) { printf("  insert 2 (insertBeforeCurr) failed: %d\n", rVal); return FAIL; }
	rVal = listFree(listPtr);
	if (rVal != SUCCESS) { printf("  failed 2 (!empty List): %d\n", rVal); return FAIL; }
	else /* (rVal == SUCCESS) */ { printf("  succeeded\n"); return WIN; }
}
