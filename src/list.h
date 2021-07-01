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

List* lInit(unsigned int num, Move* val[num]);
void lFree(List* l);
void lAppend(List* l, Move* m);
void lPop(List* l);
