#include <stdio.h>
#include "stack.h"
#include "alloc.h"

stack* stack_new() {
	stack *s = xmalloc(sizeof(stack));

	s->top = 0;

	return s;
}

void stack_push(stack *s, void *d) {
	if (s->top >= STACK_MAX_LEN)
		die("Stack overflow when pushing to stack");
	s->data[s->top] = d;
	s->top++;
}

void *stack_pop(stack *s) {
	if (s->top == 0)
		die("Stack underflow when poping from stack");
	return s->data[--s->top];
}

void stack_destroy(stack *s) {
	xfree(s);
}

void stack_print(stack *s) {
	printf("stack top at %d\n", s->top);
	for (int i = 0; i < s->top; i++)
		printf("%d: %p\n", i, s->data[i]);
}

