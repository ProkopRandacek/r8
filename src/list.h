// vim: filetype=c
#include "common.h"

typedef enum ListValueType {
	lvtGROUP;
	lvtINT;
	lvtPORTALBRIDGE;
	lvtSHAPE;
} ListValueType;

typedef struct ListNode ListNode;
typedef struct ListNode {
	void* val;
	ListNode* next;
} ListNode;

typedef struct List {
	unsigned int count;
	ListValueType type;
	ListNode* first, last;
} List;

List* listInit(unsigned int num, Move* val[num]);
void listFree(List* l);
void listAppend(List* l, Move* m);
void listPop(List* l);
