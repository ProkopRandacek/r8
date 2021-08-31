#ifndef STACK_H
#define STACK_H

#include "common.h"

typedef struct Stack {
	int top;
	int cap;
	void **data;
} Stack;

Stack* stack_new(void);
void stack_destroy(Stack *s);
void stack_print(Stack *s);
void stack_push(Stack *s, void *d);
void *stack_pop(Stack *s);
void stack_resize(Stack *s, int new_size);

#endif
