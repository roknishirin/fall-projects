// huffman.c

#include "huffman.h"
#include "code.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"
#include "stdio.h"

Node *build_tree(uint64_t hist[static ALPHABET]) {
  PriorityQueue *q = pq_create(ALPHABET);
  Node *left = NULL;
  Node *right = NULL;
  Node *parent = NULL;
  Node *node = NULL;
  Node *output = NULL;
  for (int i = 0; i < ALPHABET; i++) {
    if (hist[i] != 0) {               // if histogram value is apart of input
      node = node_create(i, hist[i]); // inserting node to pq
      enqueue(q, node);
    }
  }
  while (pq_size(q) != 1) {
    dequeue(q, &left);
    dequeue(q, &right);
    parent = node_join(left, right); // enqueing node with joined nodes
    enqueue(q, parent);
  }
  dequeue(q, &output); // obtaining the last node of the pq
  pq_delete(&q);       // cleaning up
  return output;
}

Code c; // necessary so that code isn't initialized each iteration
bool is_c_set = false; // tracking

void build_codes(Node *root, Code table[static ALPHABET]) {
  if (!is_c_set) {   // if this function has never been called before
    c = code_init(); // call code_init
    is_c_set = true;
  }
  uint8_t bit;
  bit = 0;
  if (root != NULL) {
    if (root->left == 0 && root->right == 0) {
      table[root->symbol] = c;
    } else {
      code_push_bit(&c, 0);
      build_codes(root->left, table);
      code_pop_bit(&c, &bit);

      code_push_bit(&c, 1);
      build_codes(root->right, table);
      code_pop_bit(&c, &bit);
    }
  }
}

void dump_tree(int outfile, Node *root) {
  uint8_t L = 'L'; // making variables so can be accessed
  uint8_t I = 'I'; // making varibales so can be accessed
  if (root) {
    dump_tree(outfile, root->left);
    dump_tree(outfile, root->right);

    if (root->left == NULL && root->right == NULL) {
      write_bytes(outfile, &L, 1); // push for leaf
      write_bytes(outfile, &(root->symbol), 1);
    } else {
      write_bytes(outfile, &I, 1); // push for interior nodes
    }
  }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
  Stack *stack = stack_create(nbytes);
  Node *left = NULL;
  Node *right = NULL;
  Node *parent = NULL;
  Node *node = NULL;
  Node *output = NULL;
  for (uint16_t i = 0; i < nbytes; i++) {
    if (tree[i] == 'L') {
      node = node_create(tree[i + 1], 0); // push leaf node in to the stack
      stack_push(stack, node);
      i++;
    } else if (tree[i] == 'I') {
      stack_pop(stack, &left);         // pop node one
      stack_pop(stack, &right);        // pop second node
      parent = node_join(right, left); // push combined nodes
      stack_push(stack, parent);
    }
  }
  stack_pop(stack, &output); // getting the rebuild huffman tree
  stack_delete(&stack);      // deleting for cleanup
  return output;
}

void delete_tree(Node **root) {
  if (*root) { // post-order traversal deletion
    if ((*root)->left) {
      delete_tree(&((*root)->left));
    }
    if ((*root)->right) {
      delete_tree(&((*root)->right));
    }
    node_delete(root);
    *root = NULL;
  }
}
