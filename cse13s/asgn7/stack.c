// stack.c
#include "stack.h"
#include "node.h"   // node print
#include <stdlib.h> // malloc

struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
};

Stack *stack_create(uint32_t capacity) {
  Stack *s = (Stack *)malloc(sizeof(Stack)); // allocate memory for stack object
  if (s) {
    s->top = 0;             // set top to 0 for default value
    s->capacity = capacity; // set capacity to parameter value
    s->items =
        (Node **)calloc(capacity, sizeof(Node *)); // allocate memory for array
  }
  return s;
}

void stack_delete(Stack **s) {
  if ((*s)->items) {
    free((*s)->items); // free node array
  }
  if (*s) {
    free(*s); // free stack
    *s = NULL;
  }
}

bool stack_empty(Stack *s) {
  if (s->top == 0) { // if empty (top is not above 0), return true
    return true;
  }
  return false;
}

bool stack_full(Stack *s) {
  if (s->top ==
      s->capacity) { // if full (top has reached capacity), return true
    return true;
  }
  return false;
}

uint32_t stack_size(Stack *s) { return s->top; }

bool stack_push(Stack *s, Node *n) {
  if (stack_full(s) == false) { // if stack is not full
    s->items[s->top] = n;       // set the top node to the pushed node
    s->top++;                   // increment top
    return true;
  }
  return false; // stack is full
}

bool stack_pop(Stack *s, Node **n) {
  if (stack_empty(s) == false) { // if stack is not empty
    s->top--;                    // decrement top
    *n = s->items[s->top];       // return popped node
    return true;
  }
  return false; // stack is empty
}

void stack_print(Stack *s) {
  for (uint32_t i = 0; i < s->top; i++) {
    node_print(s->items[i]);
  }
}
