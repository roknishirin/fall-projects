#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define OPTIONS "b:i:n:d:s:vh"

// initializing randstate
gmp_randstate_t state;

// public and private keys must initialization as characters for optarg
char *public = "rsa.pub", *private = "rsa.priv";

// the default seed is seconds
uint64_t min_bits = 1024, iters = 50;

int main(int argc, char **argv) {

  // public and private keys must initialization as files to use fopen()
  FILE *public_key, *private_key, *error_file = stderr;

  int opt = 0, verbose = 0, usage = 0;

  uint64_t seed = time(NULL);

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'b':
      // specifies the minimum bits needed for the public modulus n

      min_bits = strtoul(optarg, NULL, 10);
      if (min_bits < 50 || min_bits > 4096) {
        gmp_fprintf(error_file,
                    "./keygen: Number of bits must be 51-4096, not %s.\n",
                    optarg);
        fprintf(
            error_file,
            "Usage: ./keygen [options]\n"
            "  ./keygen generates a public / private key pair, placing the "
            "keys into the public and private\n"
            "  key files as specified below. The keys have a modulus (n) whose "
            "length is specified in\n"
            "  the program options.\n"
            "    -s <seed>   : Use <seed> as the random number seed. Default: "
            "time()\n"
            "    -b <bits>   : Public modulus n must have at least <bits> "
            "bits. Default: 1024\n"
            "    -i <iters>  : Run <iters> Miller-Rabin iterations for "
            "primality testing. Default: 50\n"
            "    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n"
            "    -d <pvfile> : Private key file is <pvfile>. Default: "
            "rsa.priv\n"
            "    -v          : Enable verbose output.\n"
            "    -h          : Display program synopsis and usage.\n");
        return -1;
      }

      break;

    case 'i':
      // specifies the number of Miller-Rabin iterations for testing primes

      iters = strtoul(optarg, NULL, 10);
      if (iters < 1 || iters > 500) {
        gmp_fprintf(error_file,
                    "./keygen: Number of iterations must be 1-500, not %s.\n",
                    optarg);
        fprintf(
            error_file,
            "Usage: ./keygen [options]\n"
            "  ./keygen generates a public / private key pair, placing the "
            "keys into the public and private\n"
            "  key files as specified below. The keys have a modulus (n) whose "
            "length is specified in\n"
            "  the program options.\n"
            "    -s <seed>   : Use <seed> as the random number seed. Default: "
            "time()\n"
            "    -b <bits>   : Public modulus n must have at least <bits> "
            "bits. Default: 1024\n"
            "    -i <iters>  : Run <iters> Miller-Rabin iterations for "
            "primality testing. Default: 50\n"
            "    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n"
            "    -d <pvfile> : Private key file is <pvfile>. Default: "
            "rsa.priv\n"
            "    -v          : Enable verbose output.\n"
            "    -h          : Display program synopsis and usage.\n");
        return -1;
      }

      break;

    case 'n':
      // specifies the public key file

      // open for reading and writing
      public
      = optarg;
      break;

    case 'd':
      // specifies the private key file

      // open for reading and writing
      private
      = optarg;
      break;

    case 's':
      // specifies the random seed for the random state initialization

      seed = strtoul(optarg, NULL, 10);
      break;

    case 'v':
      // enables verbose output

      verbose = 1;
      break;

    case 'h':
      // enables printing usage/synopsis output
      usage = 1;
      break;

    default:
      fprintf(
          error_file,
          "Usage: ./keygen [options]\n"
          "  ./keygen generates a public / private key pair, placing the keys "
          "into the public and private\n"
          "  key files as specified below. The keys have a modulus (n) whose "
          "length is specified in\n"
          "  the program options.\n"
          "    -s <seed>   : Use <seed> as the random number seed. Default: "
          "time()\n"
          "    -b <bits>   : Public modulus n must have at least <bits> bits. "
          "Default: 1024\n"
          "    -i <iters>  : Run <iters> Miller-Rabin iterations for primality "
          "testing. Default: 50\n"
          "    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n"
          "    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n"
          "    -v          : Enable verbose output.\n"
          "    -h          : Display program synopsis and usage.\n");
      return -1;

      return 0;
    }
  }

  if (usage == 1) {
    fprintf(
        error_file,
        "Usage: ./keygen [options]\n"
        "  ./keygen generates a public / private key pair, placing the keys "
        "into the public and private\n"
        "  key files as specified below. The keys have a modulus (n) whose "
        "length is specified in\n"
        "  the program options.\n"
        "    -s <seed>   : Use <seed> as the random number seed. Default: "
        "time()\n"
        "    -b <bits>   : Public modulus n must have at least <bits> bits. "
        "Default: 1024\n"
        "    -i <iters>  : Run <iters> Miller-Rabin iterations for primality "
        "testing. Default: 50\n"
        "    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n"
        "    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n"
        "    -v          : Enable verbose output.\n"
        "    -h          : Display program synopsis and usage.\n");

    return 0;
  }

  // opening for reading and writing
  public_key = fopen(public, "w"); // making public key

  if (!public_key) { // Print a helpful error and exit the program in the event
                     // of a failure
    fprintf(error_file, "public key is not found\n");
    fclose(error_file);
    return -1;
  }

  private_key = fopen(private, "w"); // making private key

  if (!private_key) { // Print a helpful error and exit the program in the event
                      // of a failure
    fprintf(error_file, "private key is not found\n");
    fclose(public_key); // only close public
    fclose(error_file);
    return -1;
  }

  // setting the permissions for the user, and no permissions for anyone else
  fchmod(fileno(private_key), 0600);

  // Initialize the random state using the seed "seed"
  randstate_init(seed);

  // Make the public and private keys
  mpz_t p, q, n, e, d, sign, username;
  mpz_inits(p, q, n, e, d, sign, username, NULL);

  rsa_make_pub(p, q, n, e, min_bits, iters); // making public key
  rsa_make_priv(d, e, p, q);                 // making private key

  // Get the current user’s name as a string
  char *user = getenv("USER");
  // if user is null, error messgae
  mpz_set_str(username, user, 62); // Convert the username into an mpz_t
  rsa_sign(sign, username, d, n);  // Compute the signature of the username

  // Write the computed public and private key to their respective files
  rsa_write_pub(n, e, sign, user, public_key); // writing public key
  rsa_write_priv(n, d, private_key);           // writing private key

  // If verbose output is enabled
  // the mpz_t values should be printed with information about the number of
  // bits
  if (verbose == 1) {
    fprintf(error_file, "username: %s\n", user); // printing user
    gmp_fprintf(error_file, "user signature (%lu bits): %Zd\n",
                mpz_sizeinbase(sign, 2), sign); // printing sign
    gmp_fprintf(error_file, "p (%lu bits): %Zd\n", mpz_sizeinbase(p, 2),
                p); // printing p
    gmp_fprintf(error_file, "q (%lu bits): %Zd\n", mpz_sizeinbase(q, 2),
                q); // printing q
    gmp_fprintf(error_file, "n - modulus (%lu bits): %Zd\n",
                mpz_sizeinbase(n, 2), n); // printing n
    gmp_fprintf(error_file, "e - public exponent (%lu bits): %Zd\n",
                mpz_sizeinbase(e, 2), e); // printing e
    gmp_fprintf(error_file, "d - private exponent (%lu bits): %Zd\n",
                mpz_sizeinbase(d, 2), d); // printing d
  }

  fclose(public_key);  // closing public key
  fclose(private_key); // closing private key
  fclose(error_file);  // closing error file

  randstate_clear(); // clearing random state

  mpz_clears(p, q, n, e, d, sign, username, NULL);

  return 0;
}
