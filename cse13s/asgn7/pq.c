// pq.c file

#include "pq.h"
#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

struct PriorityQueue {
  uint32_t size;
  uint32_t capacity;
  Node **inside;
};

PriorityQueue *pq_create(uint32_t capacity) {
  // The constructor for a priority queue
  PriorityQueue *q = (PriorityQueue *)malloc(
      sizeof(PriorityQueue)); // allocating memory for priority queue
  if (q) {
    q->inside = (Node **)calloc(
        capacity, sizeof(Node *)); // allocating memory for node array
    q->size = 0;                   // set the size variable as 0 for the default
    q->capacity = capacity;        // set the capacity to the parameter value
  }
  return q;
}

void pq_delete(PriorityQueue **q) {
  // The destructor for a priority queue.
  // remove the nodes
  if ((*q)->inside) {
    free((*q)->inside); // free the node array
    (*q)->inside = NULL;
  }
  free(*q); // free the priority queue object
  *q = NULL;
}

bool pq_empty(PriorityQueue *q) {
  // Returns true if the priority queue is empty and false otherwise.
  if (q->size == 0) { // if empty
    return true;
  }
  return false; // not empty
}

bool pq_full(PriorityQueue *q) {
  // Returns true if the priority queue is full and false otherwise.
  if (q->size == q->capacity) { // full
    return true;
  }
  return false; // not full
}

uint32_t pq_size(PriorityQueue *q) { return q->size; }

/*****HEAP SORT ****/
void swap(Node **n, Node **m) {
  // swapping two nodes together
  Node *temp = *n;
  *n = *m;
  *m = temp;
}

uint32_t l_child(uint32_t n) {
  // return left child index
  return (2 * n + 1);
}

uint32_t r_child(uint32_t n) {
  // return right child index
  return (2 * n + 2);
}

uint32_t parent(uint32_t n) {
  // return parent index
  return ((n - 1) / 2);
}

void up_heap(PriorityQueue *q, uint32_t i) {
  while (i > 0 && !node_cmp(q->inside[i], q->inside[parent(i)])) {
    swap(&(q->inside[i]), &(q->inside[parent(i)]));
    i = parent(i);
  }
}

void down_heap(PriorityQueue *q) {
  uint32_t n = 0, smaller;
  while (l_child(n) < q->size) {
    if (r_child(n) == q->size) {
      smaller = l_child(n);
    } else {
      if (node_cmp(q->inside[l_child(n)], q->inside[r_child(n)])) {
        smaller = r_child(n);
      } else {
        smaller = l_child(n);
      }
    }
    if (node_cmp(q->inside[smaller], q->inside[n])) {
      break;
    }
    swap(&(q->inside[n]), &(q->inside[smaller]));
    n = smaller;
  }
}
/*****END HEAP SORT***/

bool enqueue(PriorityQueue *q, Node *n) {
  // Enqueues a node into the priority queue
  if (pq_full(q)) { // if full, return false
    return false;
  }

  q->inside[q->size] = n; // set the node to the top node
  up_heap(q, pq_size(q)); // fix the heap
  q->size++;              // increment the size since a node has been added
  return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
  if (pq_empty(q)) { // if empty, return false
    return false;
  }

  *n = q->inside[0]; // return the popped node

  // set the node from the front to the tail pointer
  q->inside[0] = q->inside[q->size - 1];

  q->size--;    // decrease the size since a node has been taken away
  down_heap(q); // fix the heap
  return true;
}

void pq_print(PriorityQueue *q) {
  printf("size: %d\n", pq_size(q));
  for (uint32_t i = 0; i < pq_size(q); i++) {
    if (q->inside[i] != NULL) {
      node_print(q->inside[i]);
    }
  }
}
