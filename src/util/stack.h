#ifndef STACK_H
#define STACK_H

#include "common.h"

#define STACK_MAX_LEN 8

typedef struct stack {
	void *data[STACK_MAX_LEN];
	int top;
} stack;

stack* stack_new();
void stack_destroy(stack *s);
void stack_print(stack *s);
void stack_push(stack *s, void *d);
void *stack_pop(stack *s);

#endif
