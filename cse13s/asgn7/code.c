// code.c

#include "code.h"
#include "defines.h"

#include <stdio.h>

Code code_init(void) {
  // looks this way because it is not a pointer
  Code c;    // create a new Code on the stack
  c.top = 0; // setting top to 0
  // zeroing out the array of bits
  for (int i = 0; i < MAX_CODE_SIZE; i++) {
    c.bits[i] = 0;
  }
  return c;
}

uint32_t code_size(Code *c) { return c->top; }

bool code_empty(Code *c) {
  // return true if the code is empty
  if (code_size(c) == 0) {
    return true;
  }
  return false;
}

bool code_full(Code *c) {
  // Returns true if the Code is full and false otherwise
  if (code_size(c) == ALPHABET) {
    return true;
  }
  return false;
}

bool code_set_bit(Code *c, uint32_t i) {
  // Sets the bit at index i in the Code, setting it to 1
  // If i is out of range, return false.
  // otherwise return true to indicate success
  if (i >= 0 && i < ALPHABET) {
    uint8_t one = 1;
    // set the bit within the byte i % 8
    // or the vector
    c->bits[i / 8] |= (one << i % 8);
    return true;
  }
  return false;
}

bool code_clr_bit(Code *c, uint32_t i) {
  if (i >= 0 && i < ALPHABET) {
    uint8_t one = 1;
    c->bits[i / 8] &= ~(one << i % 8);
    return true;
  }
  return false;
}

bool code_get_bit(Code *c, uint32_t i) {
  // Gets the bit at index i in the Code
  if (i >= 0 && i < ALPHABET) {
    uint8_t one = 1;
    if (((c->bits[i / 8] >> i % 8) & one) == 1) {
      return true;
    }
  }
  return false;
}

bool code_push_bit(Code *c, uint8_t bit) {
  // Pushes a bit onto the Code

  // Returns false if the Code is full prior to pushing it
  if (code_full(c)) {
    return false;
  }
  if (bit == 1) {
    code_set_bit(c, c->top);
  } else {
    code_clr_bit(c, c->top);
  }
  c->top++;
  return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
  // Pops a bit off the Code
  if (code_size(c) == 0) { // checking if empty
    return false;
  }
  if (code_get_bit(c, (c->top)--)) {
    *bit = code_get_bit(c, c->top);
    code_clr_bit(c, c->top);
    return true;
  }
  return false;
}

void code_print(Code *c) { printf("c->top: %d\n", c->top); }
