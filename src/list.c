#include "list.h"

List* listInit(unsigned int num, Move* val[num]) {
	if (num <= 0) {
		printf("%s - list length must be greater than zero (is %d)\n", __func__, num);
		exit(1);
	}
	List*     l = dmalloc(sizeof(List));
	ListNode* n = dmalloc(sizeof(ListNode));
	n->m     = *val[0];
	n->next  = NULL;
	l->first = n;
	l->last  = n;
	l->count = 1;
	for (unsigned int i = 1; i < num; i++)
		listAppend(l, val[i]);
	return l;
}

void listFree(List* l) {
	ListNode* pos = l->first;
	while (1) {
		if (pos == NULL) break;
		if (pos->next == NULL) break;
		ListNode* tmp = pos->next;
		dfree(pos);
		pos = tmp;
	}
	if (pos != 0)
		dfree(pos);
	dfree(l);
}

void listAppend(List* l, Move* m) {
	l->last->next = dmalloc(sizeof(ListNode)); // create new node at the end
	l->last->next->next = NULL;
	l->last->next->m = *m; // paste the value
	l->last = l->last->next; // move the end pointer
	l->count++;
}

void listPop(List* l) { // deletes first element
	ListNode* pop = l->first;
	l->first = l->first->next;
	l->count--;
	dfree(pop);
}

