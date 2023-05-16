// node.c

#include "node.h"
#include <stdint.h> // for uint32_T
#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

// mimic strlen function
// from lecture 5 slides provided by professor
uint64_t mimic_strlen(char *s) {
  size_t val = 0;

  do {
    val++;
  } while (s[val] != 0);

  return val;
}

// mimic strdup function
// from lecture 5 slides provided by professor
char *my_strdup(char *s) {
  size_t s_length = mimic_strlen(s);
  char *copy = calloc(s_length + 1, sizeof(char));
  for (size_t i = 0; i < mimic_strlen(s); i++) {
    copy[i] = s[i];
  }
  return copy;
}

Node *node_create(char *oldspeak, char *newspeak) {
  // the constructor for node
  Node *n = (Node *)calloc(1, sizeof(Node));
  // since some nodes can be NULL, we need to check for their cases
  if (n) {
    if (oldspeak == NULL) { // if no oldspeak, return NULL
      n->oldspeak = NULL;
    } else {
      n->oldspeak = my_strdup(oldspeak);
    }
    if (newspeak == NULL) { // if no newspeak, return NULL
      n->newspeak = NULL;
    } else {
      n->newspeak = my_strdup(newspeak);
    }
  }
  return n;
}

void node_delete(Node **n) {
  // the detructor for a node
  // also need to check if node exists in order to free
  if ((*n)->oldspeak != NULL) {
    free((*n)->oldspeak); // free the memory allocated to oldspeak
  }
  if ((*n)->newspeak != NULL) {
    free((*n)->newspeak); // free the memory allocated to newspeak
  }
  free(*n);  // free node n
  *n = NULL; // pointer to the node is set to NULL
}

void node_print(Node *n) {
  if (n == NULL) {
    printf("NULL\n");
    return;
  }
  // if the node n contains oldspeak and newspeak
  if (n->oldspeak != NULL && n->newspeak != NULL) {
    printf("%s -> %s\n", n->oldspeak, n->newspeak);
  }
  // if the node n contains only oldspeak
  if (n->oldspeak != NULL && n->newspeak == NULL) {
    printf("%s\n", n->oldspeak);
  }
}
