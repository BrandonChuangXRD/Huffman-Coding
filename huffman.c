#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "code.h"
#include "defines.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

Node *build_tree(uint64_t hist[static ALPHABET]) {
  // make nodes and store in priority queue
  PriorityQueue *prior_q = pq_create(ALPHABET);
  for (int i = 0; i < ALPHABET; i++) {
    // dont create a code if its never used
    if (hist[i] != 0) {
      enqueue(prior_q, node_create(i, hist[i]));
    }
  }

  // break if dequeue cant dequeue 2 values (size is 1)
  while (pq_size(prior_q) > 1) {
    Node *left;
    Node *right;
    dequeue(prior_q, &left);
    dequeue(prior_q, &right);

    Node *parent = node_join(left, right);
    enqueue(prior_q, parent);
  }
  Node *root;
  dequeue(prior_q, &root);
  pq_delete(&prior_q);
  return root;
}

void search_build(Node *branches, Code table[static ALPHABET], Code *path) {
  // if root has no left child, it has no children. (base case)
  uint8_t dummy = 0; // this is to fill the parameter for code_pop
  if (branches->left == NULL) {
    table[branches->symbol] = *path;
  } else {

    // recurse on the left side of the tree
    if (branches->left != NULL) {
      code_push_bit(path, 0);
      search_build(branches->left, table, path);
    }

    // recurse on the right side of the tree
    if (branches->right != NULL) {
      code_push_bit(path, 1);
      search_build(branches->right, table, path);
    }
  }
  code_pop_bit(path, &dummy);
}

void build_codes(Node *root, Code table[static ALPHABET]) {
  // binary tree searching and record any findings into alphabet
  Code path = code_init();
  search_build(root, table, &path);
}

void dump_tree(int outfile, Node *root) {
  if (root != NULL) {
    dump_tree(outfile, root->left);
    dump_tree(outfile, root->right);

    if (root->left == NULL) {
      write(outfile, "L", 1);
      write(outfile, &root->symbol, 1);
      bytes_written += 2;
    } else {
      write(outfile, "I", 1);
      bytes_written++;
    }
  }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
  // loop through the tree
  // store the L(symbol)'s you find
  // when you hit an I
  // remove stack pop the last 2 elements
  // the first popped element goes on the right
  // the second goes on the left for node_join
  // the parent node goes into the stack

  Stack *stored = stack_create(ALPHABET);
  // loop through the dump
  for (int i = 0; i < nbytes; i++) {
    if (tree[i] == 76) {
      i++;
      stack_push(stored, node_create((char)tree[i], 0));
    } else if (tree[i] ==
               73) { // this should always be true if the first else fails
      Node *right;
      Node *left;
      stack_pop(stored, &right);
      stack_pop(stored, &left);
      Node *parent = node_join(left, right);
      stack_push(stored, parent);

    } else {
      printf("something probably went wrong at %d\n", i);
      i++;
    }
  }
  Node *root;
  stack_pop(stored, &root);
  stack_delete(&stored);
  return root;
}

void delete_tree(Node **root) {
  // recurse through the whole tree and delete everything

  if (*root && (*root)->left != NULL) {
    delete_tree(&(*root)->left);
    delete_tree(&(*root)->right);
  }
  node_delete(&(*root));
  *root = NULL;
}
