#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "pq.h"

struct PriorityQueue {
  uint32_t capacity;
  uint32_t top;
  Node **items;
};

PriorityQueue *pq_create(uint32_t capacity) {
  PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  if (q) {
    q->top = 0;
    q->capacity = capacity;
    q->items = (Node **)calloc(capacity, sizeof(Node *));
    if (!q->items) {
      free(q);
      q = NULL;
    }
  }
  return q;
}

void pq_delete(PriorityQueue **q) {
  if (*q && (*q)->items) {
    free((*q)->items);
    free(*q);
    *q = NULL;
  }
}

bool pq_empty(PriorityQueue *q) { return q->top == 0; }

bool pq_full(PriorityQueue *q) { return q->top == q->capacity; }

uint32_t pq_size(PriorityQueue *q) { return q->top; }

bool enqueue(PriorityQueue *q, Node *n) {

  if (q->capacity == q->top) {
    return false;
  }
  // push the value (order doesn't really matter until you start dequeue'ing, i
  // think)
  q->items[q->top] = n;
  q->top++;
  return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
  // TODO this should be pretty hard
  // nothing to do if the queue is empty
  if (q->top == 0) {
    return false;
  }

  // use insertion sort to sort the queue (largest frequency first)
  for (uint32_t i = 1; i < q->top - 1; i++) {
    uint32_t j = i;
    Node *temp = q->items[i];
    while (j > 0 && temp->frequency > q->items[j - 1]->frequency) {
      q->items[j] = q->items[j - 1];
      j--;
    }
    q->items[j] = temp;
  }
  // pop the last value in the stack (smallest frequency)
  q->top--;
  // set n to the pointer
  *n = q->items[q->top];

  return true;
}

void pq_print(PriorityQueue *q) {
  printf("QUEUE PRINTOUT:\n");
  printf("capacity: %u\n", q->capacity);
  printf("top/size: %u\n", q->top);

  printf("items:\n");
  for (uint32_t i = 0; i < q->top; i++) {
    node_print(q->items[i]);
    printf("frequency: %lu", q->items[i]->frequency);
    printf("\n");
  }
  printf("\n");
}
