// ht.c

#include "ht.h"
#include "city.h"
#include "ll.h"

#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

struct HashTable {
  uint64_t salt;
  uint32_t size;
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_examined;
  bool mtf;
  LinkedList **lists;
};

HashTable *ht_create(uint32_t size, bool mtf) {
  // constructor of the hash table
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht != NULL) {
    ht->mtf = mtf;
    ht->salt = 0x9846e4f157fe8840;
    ht->n_hits = ht->n_misses = ht->n_examined = 0;
    ht->n_keys = 0;
    ht->size = size;
    ht->lists = (LinkedList **)calloc(size, sizeof(LinkedList *));
    if (!ht->lists) {
      free(ht);
      ht = NULL;
    }
  }
  return ht;
}

void ht_delete(HashTable **ht) {
  // psuedocode provided by professor miller's slides
  // the deconstructor for the hash table
  // each of the linked lists in lists is freed

  if (*ht != NULL) {
    if ((*ht)->lists != NULL) {
      for (uint32_t i = 0; i < (*ht)->size; i++) {
        if ((*ht)->lists[i] != NULL) {
          ll_delete(&((*ht)->lists[i]));
        }
      }
      free((*ht)->lists);
      (*ht)->lists = NULL;
    }
    free(*ht);
    *ht = NULL;
  }
}

uint32_t ht_size(HashTable *ht) {
  // returns the hash table's size
  return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
  // searches for an entry in the hash table that contains oldspeak
  // the index of the indexed list to perform a look-up is calculated by hashing
  // the oldspeak if the node is found, the node is returned, else a NULL
  // pointer is returned

  // pseudocode provided by professor millers slides

  uint32_t index =
      hash(ht->salt, oldspeak) % ht->size; // hash value % size of hash table
  if (ht->lists[index] == NULL) {
    ht->n_misses++;
    return NULL;
  }
  uint32_t links = 0, seeks = 0;
  ll_stats(&seeks, &links);
  uint32_t new_links = 0;

  Node *output = ll_lookup((ht)->lists[index], oldspeak);
  ll_stats(&seeks, &new_links);
  ht->n_examined += (new_links - links);
  if (output == NULL) {
    ht->n_misses++;
    return NULL;
  }
  ht->n_hits++;

  return output;
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
  // inserts the specified oldspeak and its corresponding newspeak translation
  // into the hash table the index of the linked list to insert into is
  // calculated by hashing the oldspeak if the linked list that should be
  // inserted into hasn't been initialized yet, create it first before insertin
  // the oldspeak and newspeak

  uint32_t index =
      hash(ht->salt, oldspeak) % ht->size; // hash value % size of hash table

  if (ht->lists[index] == NULL) {
    ht->lists[index] = ll_create(ht->mtf);
    ht->n_keys++;
  } else if (ll_lookup(ht->lists[index], oldspeak) == NULL) {
    ht->n_keys++;
  }

  ll_insert((ht)->lists[index], oldspeak, newspeak);
}

uint32_t ht_count(HashTable *ht) {
  // returns the number of non-NULL linked lists in the hash table
  uint32_t count = 0;
  for (uint32_t i = 0; i < ht->size; i++) {
    if (ht->lists[i] != NULL) {
      count += 1;
    }
  }
  return count;
}

void ht_print(HashTable *ht) {
  for (uint32_t i = 0; i < ht->size; i++) {
    if (ht->lists[i] != NULL) {
      ll_print(ht->lists[i]);
    }
  }
}

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
  *nk = ht->n_keys;
  *nh = ht->n_hits;
  *nm = ht->n_misses;
  *ne = ht->n_examined;
}
