#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

#include <unistd.h>

#define OPTIONS "i:o:n:vh"

// initializing randstate
gmp_randstate_t state;

int main(int argc, char **argv) {

  int opt = 0, verbose = 0, usage = 0;

  char user[300] = {0}, *public = "rsa.pub";

  // setting the default values
  FILE *error_file = stderr, *input_file = stdin, *output_file = stdout,
       *public_key;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {

    case 'i':
      // specifies the input file to encrypt
      // open for reading
      input_file = fopen(optarg, "r");
      break;

    case 'o':
      // specifies the output file to encrypt
      // open for reading and writing
      output_file = fopen(optarg, "w");
      break;

    case 'n':
      // specifies the file containing the public key
      public
      = optarg;
      break;

    case 'v':
      // enables verbose output
      verbose = 1;
      break;

    case 'h':
      // enables usage/synopsis output
      usage = 1;
      break;

    default:
      fprintf(
          error_file,
          "Usage: ./encrypt [options]\n"
          "  ./encrypt encrypts an input file using the specified public key "
          "file,\n"
          "  writing the result to the specified output file.\n"
          "    -i <infile> : Read input from <infile>. Default: standard "
          "input.\n"
          "    -o <outfile>: Write output to <outfile>. Default: standard "
          "output.\n"
          "    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n"
          "    -v          : Enable verbose output.\n"
          "    -h          : Display program synopsis and usage.\n");
      return -1;

      return 0;
    }
  }

  if (usage == 1) {
    fprintf(
        error_file,
        "Usage: ./encrypt [options]\n"
        "  ./encrypt encrypts an input file using the specified public key "
        "file,\n"
        "  writing the result to the specified output file.\n"
        "    -i <infile> : Read input from <infile>. Default: standard input.\n"
        "    -o <outfile>: Write output to <outfile>. Default: standard "
        "output.\n"
        "    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n"
        "    -v          : Enable verbose output.\n"
        "    -h          : Display program synopsis and usage.\n");
    return 0;
  }

  // Open the public key file
  public_key = fopen(public, "r");

  // Print a helpful error and exit the program in the event of failure
  if (!public_key) {
    fprintf(error_file, "public key is not found\n");
    fclose(error_file);
    return 1;
  }

  // reading the public key
  mpz_t n, e, sign, username;
  mpz_inits(n, e, sign, username, NULL);

  rsa_read_pub(n, e, sign, user, public_key);

  // if verbose output is enabled
  // the mpz_t values should be printed with information about the number of
  // bits
  if (verbose == 1) {
    fprintf(error_file, "username: %s\n", user);
    gmp_fprintf(error_file, "user signature (%lu bits): %Zd\n",
                mpz_sizeinbase(sign, 2), sign);
    gmp_fprintf(error_file, "n - modulus (%lu bits): %Zd\n",
                mpz_sizeinbase(n, 2), n);
    gmp_fprintf(error_file, "e - public exponent (%lu bits):  %Zd\n",
                mpz_sizeinbase(e, 2), e);
  }

  // convert username into a mpz_t
  mpz_set_str(username, user, 62);

  // RSA signature check
  if (!rsa_verify(username, sign, e, n)) {
    fprintf(error_file, "cannot verify the user's signature\n");
    mpz_clears(n, e, sign, username, NULL);
    fclose(public_key);
    fclose(input_file);
    fclose(output_file);
    fclose(error_file);
    return -1;
  }

  // encrypt the file
  rsa_encrypt_file(input_file, output_file, n, e);

  // cleaning up
  fclose(public_key);
  mpz_clears(n, e, sign, username, NULL);
  fclose(input_file);
  fclose(output_file);
  fclose(error_file);

  return 0;
}
