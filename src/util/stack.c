#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "alloc.h"
#include "log.h"

Stack* stack_new() {
	Stack *s = xmalloc(sizeof(Stack));

	s->top = 0;
	s->cap = 64;
	s->data = xmalloc(sizeof(void*) * s->cap);

	return s;
}

void stack_push(Stack *s, void *d) {
	// expand stack if needed
	if (s->top >= s->cap)
		stack_resize(s, s->cap * 2);

	s->data[s->top] = d;
	s->top++;
}

void *stack_pop(Stack *s) {
	if (s->top < (s->cap / 4))
		stack_resize(s, s->cap / 2);

	if (s->top == 0)
		die("Stack underflow when poping from stack");
	return s->data[--s->top];
}

void stack_destroy(Stack *s) {
	xfree(s);
}

void stack_print(Stack *s) {
	msg("stack top at %d, cap %d\n", s->top, s->cap);
	for (int i = 0; i < s->top; i++)
		msg("%d: %p\n", i, s->data[i]);
}

void stack_resize(Stack *s, int new_size) {
	void **new = xrealloc(s->data, sizeof(void*) * new_size);
	s->data = new;
	s->cap = new_size;
}
