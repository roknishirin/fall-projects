#include "numtheory.h"		// }
#include "randstate.h"		// }
#include <stdbool.h>		// }
#include <stdint.h>		// }
#include <stdio.h>		// } first block will be sorted

#include <gmp.h> // will not be sorted with previous blocks

extern gmp_randstate_t state;

void gcd(mpz_t d, mpz_t a, mpz_t b) {
  // making copies to manipulate
  mpz_t B, A;
  mpz_inits(B, A, NULL);

  mpz_set(B, b);
  mpz_set(A, a);

  // while b ≠ 0
  while (mpz_cmp_ui(B, 0) != 0) {
    mpz_t temp;
    mpz_init(temp);

    mpz_set(temp, B); // t <- b
    mpz_mod(B, A, B); // b <- a mod b
    mpz_set(A, temp); // a <- t

    mpz_clear(temp);
  }
  mpz_set(d, A);
  mpz_clears(A, B, NULL);
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
  // Computes the inverse i of a modulo n. In the event that a modular inverse
  // cannot be found, set i to 0.

  // initializing copies
  mpz_t R, R_prime, R_copy, T, T_copy, T_prime, Q;
  mpz_inits(R, R_prime, R_copy, T, T_copy, T_prime, Q, NULL);

  // (r, r') <- (n, a)
  mpz_set(R, n);
  mpz_set(R_prime, a);

  // (t, t') <- (0, 1)
  mpz_set_ui(T, 0);
  mpz_set_ui(T_prime, 1);

  while (mpz_cmp_ui(R_prime, 0) != 0) {
    mpz_fdiv_q(Q, R, R_prime);

    // making copies
    mpz_set(R_copy, R);

    // (r, r') <- (r', r - q x r')
    mpz_set(R, R_prime);

    mpz_mul(R_prime, Q, R_prime);
    mpz_sub(R_prime, R_copy, R_prime);

    // making copies
    mpz_set(T_copy, T);

    // (t, t') <- (t', t- 1 x r')
    mpz_set(T, T_prime);

    mpz_mul(T_prime, Q, T_prime);
    mpz_sub(T_prime, T_copy, T_prime);

  } // if r > 1
  if (mpz_cmp_ui(R, 1) > 0) {
    mpz_set_ui(T, 0);

    // if t < 0
  }
  if (mpz_cmp_ui(T, 0) < 0) {
    // t <- t + n
    mpz_add(T, T, n);
  }
  mpz_set(o, T);
  mpz_clears(R, R_prime, R_copy, T, T_copy, T_prime, Q, NULL);
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
  // computing base raised to the exponent power modulo modulus,and storing the
  // computed result in out

  // making variables
  mpz_t D, v, p;
  mpz_inits(D, v, p, NULL);

  // initializing copies
  mpz_set(D, d);

  // v <- 1
  mpz_set_ui(v, 1);

  // p <- a
  mpz_set(p, a);

  while (mpz_cmp_ui(D, 0) > 0) {
    // odd(d)
    if (mpz_odd_p(D) != 0) {
      // v <- (v x p) mod n
      mpz_mul(v, v, p);
      mpz_mod(v, v, n);
    }
    // p <- (p x p) mod n
    mpz_mul(p, p, p);
    mpz_mod(p, p, n);

    // d <- (d /  2)
    mpz_fdiv_q_ui(D, D, 2);
  }
  mpz_set(o, v);
  mpz_clears(D, v, p, NULL);
}

// concept of function was discussed with tutor lev
void totient(mpz_t r, mpz_t s, mpz_t n) {
  mpz_t n_copy;
  mpz_inits(n_copy, NULL);

  // doing the assigning
  mpz_set(n_copy, n);
  uint64_t temp_mod = mpz_even_p(n_copy);

  while (temp_mod != 0) {
    mpz_add_ui(s, s, 1);
    mpz_fdiv_q_ui(n_copy, n_copy, 2);
    temp_mod = mpz_even_p(n_copy);
  }
  mpz_set(r, n_copy);
  mpz_clears(n_copy, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
  // Conducts the Miller-Rabin primality test to indicate whether or not n is
  // prime using iters number of Miller-Rabin iterations. This function is
  // needed when creating the two large primes p and q in RSA, verifying if a
  // large integer is a prime.

  // base case for when 1-3 return true

  if (mpz_cmp_ui(n, 1) <= 0) {
    return false;
  }

  if (mpz_cmp_ui(n, 4) < 0) {
    return true;
  }

  // checking for even numbers > 2
  if (mpz_even_p(n) > 0) {
    return false;
  }

  // making variables
  mpz_t a, s, r, n_minus_3, two, n_minus_1, y, j;
  mpz_inits(a, s, r, n_minus_3, two, n_minus_1, y, j, NULL);

  // initializing copies
  mpz_sub_ui(n_minus_3, n, 3);
  mpz_sub_ui(n_minus_1, n, 1);

  // write n−1 = 2^s r such that r is odd
  totient(r, s, n_minus_1);

  // mpz_set_ui (i, 1);
  mpz_set_ui(two, 2);

  mpz_sub_ui(s, s, 1);

  for (uint64_t i = 1; i < iters; i++) {
    // random stuff
    mpz_urandomm(a, state, n_minus_3);
    mpz_add_ui(a, a, 2);
    pow_mod(y, a, r, n);
    if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n_minus_1) != 0)) {
      mpz_set_ui(j, 1);
      while (mpz_cmp(j, s) < 1 && mpz_cmp(y, n_minus_1) != 0) {
        pow_mod(y, y, two, n);
        if (mpz_cmp_ui(y, 1) == 0) {
          mpz_clears(a, s, r, n_minus_3, n_minus_1, y, j, NULL);
          return false;
        }
        mpz_add_ui(j, j, 1);
      }
      if (mpz_cmp(y, n_minus_1) != 0) {
        mpz_clears(a, s, r, n_minus_3, n_minus_1, y, j, NULL);
        return false;
      }
    }
  }
  mpz_clears(a, s, r, n_minus_3, n_minus_1, y, j, NULL);
  return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {

  // pseudocode provided by darrel long's piazza post #261
  while (!is_prime(p, iters)) {
    // generate from 0 to 2^(bits - 1)
    mpz_urandomb(p, state, bits - 1); // initializing random variables
    // *p |= 0x1 set the first bit to 1
    mpz_setbit(p, 0); // set the 0th bits to 1 will ensure the number will be
                      // odd at all times
    // want the number generator to have at least bit number of bits
    mpz_setbit(p, bits - 1); // setting the bits - 1th adds 2^(bits - 1) to the
                             // current number
  }
}
