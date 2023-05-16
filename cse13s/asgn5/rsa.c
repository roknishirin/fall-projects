#include "rsa.h"       // } first block that is sorted
#include "numtheory.h" // }
#include "randstate.h" // }
#include <stdio.h>     // }
#include <stdlib.h>    // }

#include <gmp.h> // will not be sorted with previous blocks

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits,
                  uint64_t iters) {
  //  two large primes p and q, their product n, and the public exponent e
  // inputs are two large primes p & q
  // product n
  // public exponent e

  mpz_t greatest, exp, totient;
  mpz_inits(greatest, exp, totient, NULL);

  uint64_t exit = 0, lower_bound, p_bits, q_bits;

  //  [nbits/4,(3×nbits)/4) range
  //  lower bound is [0, n_bits / 2)
  //  adding n_bits/4 to p_bits make it equal [n_bits/4, (3 x n_bits) / 3]
  lower_bound = (nbits / 4);

  p_bits = lower_bound + (random() % (nbits / 2));
  q_bits = nbits - p_bits;

  make_prime(p, p_bits, iters);
  make_prime(q, q_bits, iters);

  mpz_mul(n, p, q); // to find the product of n

  // compute λ(n) = lcm(p−1,q−1)
  mpz_sub_ui(p, p, 1);
  mpz_sub_ui(q, q, 1);
  mpz_mul(totient, p, q);

  mpz_add_ui(p, p, 1); //
  mpz_add_ui(q, q, 1); //

  // finding the gcd of each random number
  while (exit == 0) {
    mpz_urandomb(exp, state, nbits);
    gcd(greatest, exp, totient);

    // if the greatest common denominator of exp and totient is 1, terminate
    if (mpz_cmp_ui(greatest, 1) == 0) {
      exit = 1;
    }
  }

  mpz_set(e, exp);
  mpz_clears(greatest, exp, totient, NULL);
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
  // Writes a public RSA key to pbfile
  // x to print hex
  gmp_fprintf(pbfile, "%Zx\n", n);   // printing n
  gmp_fprintf(pbfile, "%Zx\n", e);   // printing e
  gmp_fprintf(pbfile, "%Zx\n", s);   // printing s
  fprintf(pbfile, "%s\n", username); // printing username
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
  // Reads a public RSA key from pbfile.
  // x to print hex
  gmp_fscanf(pbfile, "%Zx\n", n);   // scanning n
  gmp_fscanf(pbfile, "%Zx\n", e);   // scanning e
  gmp_fscanf(pbfile, "%Zx\n", s);   // scanning s
  fscanf(pbfile, "%s\n", username); // scanning username
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
  // Creates a new RSA private key d given primes p and q and public exponent e
  mpz_t P, Q, totient;
  mpz_inits(P, Q, totient, NULL);

  // making copies
  mpz_set(P, p);
  mpz_set(Q, q);

  // λ(n) = lcm(p−1,q−1)
  // lcm = (p - 1) * ( q - 1)
  mpz_sub_ui(P, P, 1);
  mpz_sub_ui(Q, Q, 1);
  mpz_mul(totient, P, Q);

  // computing the inverse
  // the inverse of e modulo λ(n)
  mod_inverse(d, e, totient);
  mpz_clears(P, Q, totient, NULL);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
  // Writes a private RSA key to pvfile
  // x to print x
  gmp_fprintf(pvfile, "%Zx\n", n); // printing n
  gmp_fprintf(pvfile, "%Zx\n", d); // printing d
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
  // Reads a private RSA key from pvfile
  // x to print hex
  gmp_fscanf(pvfile, "%Zx\n", n); // scanning n
  gmp_fscanf(pvfile, "%Zx\n", d); // scanning d
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
  // performs RSA encryption, computing ciphertext c by encrypting message m
  // using public exponent e and modulus n
  pow_mod(c, m, e, n);
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {

  mpz_t m, encrypted;
  mpz_inits(m, encrypted, NULL);

  // k = ⌊(log2 (n)−1)/8⌋.
  // using sizeinbase to find the block size exactly (2)
  uint64_t k = mpz_sizeinbase(n, 2);
  k -= 1;
  k /= 8;

  // Dynamically allocate an array that can hold k bytes
  uint8_t *block = (uint8_t *)calloc(k, sizeof(uint8_t));

  // effectively prepends the workaround byte
  block[0] = 0xFF;

  int64_t j = 1, process = 0;

  while (process == 0) {
    // binary stream input/output
    // &block is the pointer, sizeof(uint8_t) is the size, at most k-1 bytes,
    // stream to infile using fread as conceptually talked about with tutor
    j = fread(&block[1], sizeof(uint8_t), k - 1, infile);

    // if everything has been read
    if (j == 0) {
      process = 1;
    } else {
      // j + 1 to iterate through
      // void mpz_import (mpz_t rop, size_t count, int order, size_t size, int
      // endian, size_t nails, const void *op)
      mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, block);

      // Encrypt m with rsa_encrypt()
      rsa_encrypt(encrypted, m, e, n);

      // write the encrypted number to outfile
      gmp_fprintf(outfile, "%Zx\n", encrypted);
    }
  }
  free(block);
  mpz_clears(m, encrypted, NULL);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
  // performs RSA decryption
  // computing message m by decrypting ciphertext c using private key d and
  // public modulus n D(c) = m = c^d(mod n)
  pow_mod(m, c, d, n);
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
  // Decrypts the contents of infile, writing the decrypted contents to outfile
  mpz_t message, c;
  mpz_inits(message, c, NULL);

  // calculating the block size
  uint64_t k = mpz_sizeinbase(n, 2);
  k -= 1;
  k /= 8;

  // Dynamically allocating an array that can hold k bytes
  uint8_t *block = (uint8_t *)calloc(k, sizeof(uint8_t));

  int64_t process = 0, done;
  size_t j;

  while (process == 0) {

    // saving the hexstring as a mpz_t c
    done = gmp_fscanf(infile, "%Zx\n", c);

    // needs -1 because it returns -1 if valid
    if (done == -1) {
      process = 1;
    } else {
      // decrypting
      rsa_decrypt(message, c, d, n);

      // convert c back into bytes, storing them in the allocated block
      mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, message);

      // size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE
      // *stream); Write out j−1 bytes starting from index 1 of the block to
      // outfile
      fwrite(&block[1], sizeof(uint8_t), j - 1, outfile);
    }
  }
  free(block);
  mpz_clears(message, c, NULL);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
  // s = m^d (mod n)
  // performs RSA signing,
  pow_mod(s, m, d, n);
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
  // performs RSA verification
  // returning true if signature s is verified and false otherwise
  // t = V(s) = s^e (mod n)
  mpz_t t;
  mpz_init(t);

  // t = v(s) = s^e
  pow_mod(t, s, e, n);

  // The signature is verified if and only if v is the same as the expected
  // message m.
  if (mpz_cmp(t, m) == 0) {
    mpz_clear(t);
    return true;
    // signiture is not verified
  }
  mpz_clear(t);
  return false;
}
