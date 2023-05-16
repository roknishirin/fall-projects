#include "randstate.h"
#include <stdlib.h>

extern gmp_randstate_t state;

void randstate_init(uint64_t seed) {

  // Initialize state for a Mersenne Twister algorithm
  gmp_randinit_mt(state);

  // Random State Seeding
  gmp_randseed_ui(state, seed);

  // seeding
  srandom(seed);
}

void randstate_clear(void) {

  // Free all memory occupied by state
  gmp_randclear(state);
}
