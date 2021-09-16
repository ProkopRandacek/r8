// vim: ft=c
#ifndef STACK_H
#define STACK_H

#include "common.h"

/**
 * @brief A simple stack data type implementation
 */
typedef struct Stack {
	int top; //!< The index of the topmost element on the stack.
	int cap; //!< How many elements can the stack currently hold without being reallocated.
	void **data; //!< The actual data on this stack.
} Stack;

/**
 * @brief Creates a new empty stack.
 */
Stack* stack_new(void);

/**
 * @brief Frees all memory occupied by this stack.
 */
void stack_destroy(Stack *s);

/**
 * @brief Debug prints the stack to stdout.
 */
void stack_print(Stack *s);

/**
 * @brief Pushes a new value to the top of the stack.
 *
 * @note Stack overflows are handled by expanding the stack
 */
void stack_push(Stack *s, void *d);

/**
 * @brief Pops a value from the top of the stack and returns it.
 *
 * Stack is automatically shrunk to 1/2 of its size if it's top element is less than 1/4 the allocated size of this stack.
 */
void *stack_pop(Stack *s);

/**
 * @brief Internal stack resizing function.
 */
void stack_resize(Stack *s, int new_size);

#endif

