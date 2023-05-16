// bf.c

#include "bf.h"
#include "bv.h"
#include "city.h" // to hash

#include <stdio.h>  // for print and null
#include <stdlib.h> // for malloc

#define N_HASHES 5

struct BloomFilter {
  uint64_t salts[N_HASHES]; // Salts for five hash functions
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_bits_examined;
  BitVector *filter;
};

// pseudocode provided by the assignment pdf

static uint64_t default_salts[] = {0x5adf08ae86d36f21, 0x419d292ea2ffd49e,
                                   0x50d8bb08de3818df, 0x272347aea4045dd5,
                                   0x7c8e16f768811a21};

BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->n_keys = bf->n_hits = 0;
    bf->n_misses = bf->n_bits_examined = 0;
    for (int i = 0; i < N_HASHES; i++) {
      bf->salts[i] = default_salts[i];
    }
    bf->filter = bv_create(size);
    if (bf->filter == NULL) {
      free(bf);
      bf = NULL;
    }
  }
  return bf;
}

void bf_delete(BloomFilter **bf) {
  // the destructor for a bloom filter
  // free an memory allocated by the constructor
  bv_delete(&((*bf)->filter));
  free(*bf);

  // null out the pointer that was passed in
  *bf = NULL;
}

uint32_t bf_size(BloomFilter *bf) {
  // returns the size of the bloom filter (the number of bits)
  return bv_length(bf->filter); // using length from bv for bf
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
  // takes oldspeak and inserts it into the bloom filter

  // to make each salt small enough
  uint32_t size = bf_size(bf);
  bf->n_keys += 1;

  for (int i = 0; i < 5; i++) {
    uint64_t salt = hash(bf->salts[i], oldspeak);
    bv_set_bit(bf->filter, (salt % size));
  }
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
  // probes the bloom filter for oldspeak

  uint32_t size = bf_size(bf);

  for (int i = 0; i < 5; i++) {
    uint64_t salt = hash(bf->salts[i], oldspeak);
    bf->n_bits_examined++;
    uint8_t set = bv_get_bit(bf->filter, (salt % size));
    if (set == 0) {
      bf->n_misses++;
      return false;
    }
  }
  bf->n_hits++;
  return true;
}

uint32_t bf_count(BloomFilter *bf) {
  // returns the number of set bits in the bloom filter
  // needs to iterate through the bf_size
  uint32_t counter = 0;
  for (uint32_t i = 0; i < bf_size(bf); i++) {
    // checks if its in the bloom filter
    if (bv_get_bit(bf->filter, i) == 1) {
      counter++;
    }
  }
  return counter;
}

void bf_print(BloomFilter *bf) {
  // prints out Bloom Filter
  bv_print(bf->filter); // using bv print function
}

void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
  *nk = bf->n_keys;
  *nh = bf->n_hits;
  *nm = bf->n_misses;
  *ne = bf->n_bits_examined;
  // increment them
}
