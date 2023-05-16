#include "node.h"
#include <ctype.h> // for iscntrl
#include <inttypes.h>
#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

Node *node_create(uint8_t symbol, uint64_t frequency) {
  // The constructor for a node
  Node *n = (Node *)malloc(sizeof(Node)); // allocating memory for node object
  if (n) {
    n->left = 0;              // left node
    n->right = 0;             // right node
    n->symbol = symbol;       // Sets the node’s symbol as symbol
    n->frequency = frequency; //  Sets the node’s frequency as frequency.
  }
  return n;
}

void node_delete(Node **n) {
  // The destructor for a node.
  free(*n); // freeing the node object
  *n = NULL;
}

Node *node_join(Node *left, Node *right) {
  // Joins a left child node and right child node, returning a pointer to a
  // created parent node
  Node *parent_node = node_create('$', left->frequency + right->frequency);
  parent_node->left = left;
  parent_node->right = right;
  return parent_node;
}

void node_print(Node *n) {
  // a debug function
  if (n == NULL) {
    printf("null\n");
    return;
  }
  if (iscntrl(n->symbol) || !(isprint(n->symbol))) {
    printf("0x%02" PRIx8, n->symbol);
  } else {
    printf("symbol: %c\n", n->symbol);
  }
  printf("frequency: %lu\n", n->frequency);
}

bool node_cmp(Node *n, Node *m) {
  // Compares the frequency of two nodes
  // printf("n frequency: %lu\n", n->frequency);
  // printf("m frequency: %lu\n", m->frequency);
  if (n->frequency > (m->frequency)) {
    return true;
  }
  return false;
}

void node_print_sym(Node *n) {
  // only printing symbol
  printf("symbol: %c\n", n->symbol);
}
