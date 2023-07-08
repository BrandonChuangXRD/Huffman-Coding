#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "stack.h"

struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
};

Stack *stack_create(uint32_t capacity) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  if (s) {
    s->top = 0;
    s->capacity = capacity;
    s->items = (Node **)calloc(capacity, sizeof(Node *));
    if (!s->items) {
      free(s);
      s = NULL;
    }
  }
  return s;
}

void stack_delete(Stack **s) {
  if (*s && (*s)->items) {
    free((*s)->items);
    free(*s);
    *s = NULL;
  }
}

uint32_t stack_size(Stack *s) { return s->top; }

bool stack_empty(Stack *s) { return s->top == 0; }

bool stack_full(Stack *s) { return s->capacity == s->top; }

bool stack_push(Stack *s, Node *x) {
  // check if stack is full (you can't push anything if the stack is full)
  if (s->capacity == s->top) {
    return false;
  }
  // push the value
  s->items[s->top] = x;
  s->top += 1;
  return true;
}

bool stack_pop(Stack *s, Node **x) {
  // check if stack is empty (you can't pop anything if there's nothing in the
  // stack)
  if (s->top == 0) {
    return false;
  }
  // point x to the top of the stack
  *x = s->items[s->top - 1];
  // decrement the top (effectively removing the node from the stack)
  s->top--;
  return true;
}

void stack_print(Stack *s) {
  // loop through each value of the stack and use the node_print function
  printf("STACK PRINTOUT\n");
  for (uint32_t i = 0; i < s->top; i++) {
    node_print(s->items[i]);
    printf("\n");
  }
  printf("\n");
}
