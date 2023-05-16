// ll.c
#include "ll.h"
#include "node.h"
#include <stdio.h>  // for printf
#include <stdlib.h> // for calloc
#include <string.h> // for strcmp

// psuedocode from lecture 11 slides

uint64_t links = 0;
uint64_t seeks = 0;

struct LinkedList {
  uint32_t length;
  Node *head; // Head sentinel node.
  Node *tail; // Tail sentinel node.
  bool mtf;
};

// from lecture 5 slides provided by professor
int my_strcmp(char *one, char *two) {
  // compares two strings character by character
  for (int i = 0; one[i] || two[i]; i++) {
    if (one[i] != two[i]) {
      return one[i] - two[i];
    }
  }
  return 0;
}

LinkedList *ll_create(bool mtf) {
  LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));
  if (ll) {
    // linking to the head and tail
    // initializing with exactly two sentinel nodes
    ll->head = node_create("head", "head");
    ll->tail = node_create("tail", "tail");

    // pointing nodes to each other
    (ll->head)->next = ll->tail;
    (ll->tail)->prev = ll->head;
    ll->length = 0;
    ll->mtf = mtf;
  }
  return ll;
}

void ll_delete(LinkedList **ll) {
  // each node in the linked list should be freed
  // the pointer to the linked list should be set to NULL

  Node *current = (*ll)->head;
  while (current != NULL) {
    Node *next = NULL; // save pointer to next node
    next = current->next;
    node_delete(&current);
    current = next;
  }
  free(*ll);
  *ll = NULL;
}

uint32_t ll_length(LinkedList *ll) {
  // returns the length of the linked list
  // equivalnet to the number of nodes (not including the head and tail nodes)
  return ll->length;
}

Node *ll_mtf(LinkedList *ll, Node *curr) {
  Node *next = curr->next;
  Node *prev = curr->prev;
  prev->next = next;
  next->prev = prev;
  /*
    (curr->prev)->next = curr->next;
    (curr->next)->prev = curr->prev;
  */
  // moving node you just removed to be inbetween the head and the previous's
  // first
  Node *first = (ll->head)->next;
  (ll->head)->next = curr;
  (curr->next) = first;
  first->prev = curr;
  curr->prev = ll->head;

  return ll->head;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
  // searches for a node containing oldspeak
  // if a node is found, the pointer to the node is returned
  // else a null pointer is returned

  // if a node was found and the move-to-front option was specified when
  // constructing the linked list then the found node is moved to the front of
  // the linked list

  seeks++;

  Node *curr;

  for (curr = (ll->head->next); curr != (ll->tail); curr = curr->next) {
    links++;
    if (my_strcmp(curr->oldspeak, oldspeak) == 0) {
      break;
    }
  }
  if (ll->mtf && curr != NULL && curr != ll->tail) {
    ll_mtf(ll, curr);
  }
  return curr == ll->tail ? NULL : curr;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
  // inserts a new node containing the specified oldspeak and newspeak into the
  // linked list reconnect x and z, delete y (delete node), then insert y
  if (ll_lookup(ll, oldspeak) != NULL) {
    return;
  }
  ll->length += 1; // incrementing length

  Node *first = (ll->head)->next;
  Node *n = node_create(oldspeak, newspeak); // create a node with the speaks
  (ll->head)->next = n;
  n->next = first; // point the created node at the head
  first->prev = n;
  n->prev = ll->head;
}

void ll_print(LinkedList *ll) {
  for (Node *curr = ll->head->next; curr != ll->tail; curr = curr->next) {
    node_print(curr);
  }
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {
  *n_seeks = seeks;
  *n_links = links;
}
