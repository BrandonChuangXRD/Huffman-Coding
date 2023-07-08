#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"

Node *node_create(uint8_t symbol, uint64_t frequency) {
  Node *N = (Node *)calloc(1, sizeof(Node));
  N->left = NULL;
  N->right = NULL;
  N->symbol = symbol;
  N->frequency = frequency;
  return N;
}

void node_delete(Node **n) {
  // Deletes the nodes (to prevent memory leaks) and
  // sets the pointer to null
  free(*n);
  *n = NULL;
}

Node *node_join(Node *left, Node *right) {
  // Joins the two nodes through a parent node with the symbol '$' and
  // a frequency of the two children frequencies added
  uint32_t combined_freq = left->frequency + right->frequency;
  Node *N = node_create((uint8_t)'$', combined_freq);
  N->left = left;
  N->right = right;
  return N;
}

void node_print(Node *n) {
  // Prints information about the node
  printf("NODE %c (%hu)INFORMATION:\n", (char)n->symbol, n->symbol);
  printf("Frequency: %lu\n", n->frequency);
  if (n->left != NULL) {
    printf("Has left node\n");
  } else {
    printf("Does not have left node\n");
  }
  if (n->right != NULL) {
    printf("Has right node\n");
  } else {
    printf("Does not have right node\n");
  }
  printf("\n");
}
