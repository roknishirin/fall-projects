#include "bv.h"
#include <stdio.h>  // to use printf
#include <stdlib.h> // to use malloc

struct BitVector {
  uint32_t length;
  uint64_t *vector;
};

BitVector *bv_create(uint32_t length) {
  // the constructor for a bit vector
  BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
  if (bv) {
    bv->length = length;

    // formatting taught by tutor miles
    uint32_t size = length / 64 + (length % 64 ? 1 : 0);

    // each bit of the bit vector should be initialized to 0
    bv->vector = (uint64_t *)calloc(size, sizeof(uint64_t));
  }
  if (!bv->vector) {
    return NULL;
  }
  return bv;
}

void bv_delete(BitVector **bv) {
  // the deconstructor for a bit vector
  free((*bv)->vector); // freeing the bit vector
  free(*bv);           // freeing the memory associated with the bit vector
  *bv = NULL;          // set the pointer to NULL after
}

uint32_t bv_length(BitVector *bv) {
  // returns the length of a bit vector
  return bv->length;
}

void bv_set_bit(BitVector *bv, uint32_t i) {
  // sets the ith bit in a bit vector

  // pseudocode from set.c in assignment 4

  // [i / 64] determins which index we should manipulate in
  // (1 << (i % 64)); determins which bit in the indices we choose
  uint64_t one = 1; // type casting 1 to be uint64
  bv->vector[i / 64] |= (one << (i % 64));
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
  // clears the ith bit in the bit vector

  // flipping the bits so that you get a 0 in the location that needs to be
  // cleared do & operator to succesfully clear
  uint64_t one = 1; // type casting 1 to be uint64
  bv->vector[i / 64] &= ~(one << i % 64);
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
  // returns the ith bit in the bit vector
  // format explained by tutor miles
  uint64_t one = 1;
  uint8_t ith_bit = (bv->vector[i / 64] >> i % 64) &
                    one; // returns 1 if there is a value, 0 otherwise
  return ith_bit;
}

void bv_print(BitVector *bv) {
  for (uint32_t i = 0; i < bv->length;
       i += 1) { // iterating through the length of the array
                 // printf("%lu ",bv->vector[i]);
    if (bv_get_bit(bv, i) == 1) {
      printf("%d ", 1);
    } else {
      printf("%d ", 0);
    }
  }
  printf("\n");
}
