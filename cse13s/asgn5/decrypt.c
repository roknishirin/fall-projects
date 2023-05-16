#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

#include <unistd.h>

#define OPTIONS "i:o:n:hv"

// initializing randstate
gmp_randstate_t state;

int main(int argc, char **argv) {

  int opt = 0, verbose = 0, usage = 0;

  char *private = "rsa.priv";

  // setting the default values
  FILE *error_file = stderr, *input_file = stdin, *output_file = stdout,
       *private_key;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'i':
      // specifies the input file to decrypt
      // open for reading
      input_file = fopen(optarg, "r");
      break;

    case 'o':
      // specifies the outptu file to decrypt
      // open for reaading and writing
      output_file = fopen(optarg, "w");
      break;

    case 'n':
      // specifies the file containing the private key
      private
      = optarg;

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
          "Usage: ./decrypt [options]\n"
          "  ./decrypt decrypts an input file using the specified private key "
          "file,\n"
          "  writing the result to the specified output file.\n"
          "    -i <infile> : Read input from <infile>. Default: standard "
          "input.\n"
          "    -o <outfile>: Write output to <outfile>. Default: standard "
          "output.\n"
          "    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n"
          "    -v          : Enable verbose output.\n"
          "    -h          : Display program synopsis and usage.\n");
      return -1;

      return 0;
    }
  }

  if (usage == 1) {
    fprintf(
        error_file,
        "Usage: ./decrypt [options]\n"
        "  ./decrypt decrypts an input file using the specified private key "
        "file,\n"
        "  writing the result to the specified output file.\n"
        "    -i <infile> : Read input from <infile>. Default: standard input.\n"
        "    -o <outfile>: Write output to <outfile>. Default: standard "
        "output.\n"
        "    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n"
        "    -v          : Enable verbose output.\n"
        "    -h          : Display program synopsis and usage.\n");
    return 0;
  }

  // open the private key file
  private_key = fopen(private, "r");

  // Print a helpful error and exit the program in the event of failure
  if (!private_key) {
    fprintf(error_file, "private key is not found\n");
    fclose(error_file);
    return -1;
  }

  // reading the private key
  mpz_t n, d;
  mpz_inits(n, d, NULL);

  rsa_read_priv(n, d, private_key);

  // if verbose is enabled
  // the mpz_t values should be printed with information about the number of
  // bits
  if (verbose == 1) {
    gmp_fprintf(error_file, "n - modulus (%zu bits): %Zd\n",
                mpz_sizeinbase(n, 2), n);
    gmp_fprintf(error_file, "d - private exponent (%zu bits): %Zd\n",
                mpz_sizeinbase(d, 2), d);
  }

  // decrypt the file
  rsa_decrypt_file(input_file, output_file, n, d);

  // cleaning up
  fclose(private_key);
  mpz_clears(n, d, NULL);
  fclose(input_file);
  fclose(output_file);
  fclose(error_file);

  return 0;
}
