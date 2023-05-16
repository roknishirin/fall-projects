// encode.o

#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#include <ctype.h>     // for printTree
#include <errno.h>     // for errno
#include <fcntl.h>     // for open
#include <inttypes.h>  // for printTree
#include <stdbool.h>   // bols
#include <stdio.h>     // for printf
#include <string.h>    // for strlen
#include <sys/stat.h>  // for fstat
#include <sys/types.h> // for fstat
#include <unistd.h>    // for stdin_fileno and getopt
#include <unistd.h>    // for fstat

#define OPTIONS "hi:o:v"
/*
// function for printing tree
void spaces(int i) {
  int counter = 0;
  while (counter < i) {
    fprintf(stdout, " ");
    fflush(stdout);
    counter++;
  }
}

// long discord code for printing tree
void printTree(Node *t, int depth) {
  if (t) {
    printTree(t->left, depth + 1);
    spaces(4 * depth);
    if (t->symbol != '$') {
      if (isgraph(t->symbol)) {
        fprintf(stderr, "'%c' (%" PRIu64 ")\n", t->symbol, t->frequency);
      } else {
        fprintf(stderr, "0x%02X (%" PRIu64 ")\n", t->symbol, t->frequency);
      }
    } else {
      fprintf(stderr, "$ (%" PRIu64 ")\n", t->frequency);
    }
    printTree(t->right, depth + 1);
  }
  return;
}
*/

int main(int argc, char **argv) {
  int infile = STDIN_FILENO;
  int outfile = STDOUT_FILENO;
  int opt = 0;
  int verbose = 0;

  bool bol_one = false;
  bool bol_two = false;

  // creating a temp file for stdin
  FILE *temp = tmpfile();
  int temp_num = fileno(temp);

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      fprintf(stderr,
              "SYNOPSIS\n"
              "  A Huffman encoder.\n"
              "  Compresses a file using the Huffman coding algorithm.\n\n"

              "USAGE\n"
              "  ./encode [-h] [-i infile] [-o outfile]\n\n"

              "OPTIONS\n"
              "  -h             Program usage and help.\n"
              "  -v             Print compression statistics.\n"
              "  -i infile      Input file to compress.\n"
              "  -o outfile     Output of compressed data.\n");
      return 0;

    case 'i':
      infile = open(optarg, O_RDONLY);
      bol_one = true;
      break;

    case 'o':
      outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC);
      bol_two = true;
      break;

    case 'v':
      verbose = 1;
      break;

    default:
      fprintf(stderr,
              "SYNOPSIS\n"
              "  A Huffman encoder.\n"
              "  Compresses a file using the Huffman coding algorithm.\n\n"

              "USAGE\n"
              "  ./encode [-h] [-i infile] [-o outfile]\n\n"

              "OPTIONS\n"
              "  -h             Program usage and help.\n"
              "  -v             Print compression statistics.\n"
              "  -i infile      Input file to compress.\n"
              "  -o outfile     Output of compressed data.\n");
      return -1;
    }
  }

  uint64_t histogram[ALPHABET] = {0}; // making histogram
  uint32_t characters = 0;            // counting special symbols
  uint8_t buf[BLOCK];                 // making buffer with block

  if (histogram[0] == 0) { // if hist[0] or hist[1] == 0, equal to 1
    histogram[0] = 1;
  }

  if (histogram[1] == 0) {
    histogram[1] = 1;
  }

  uint8_t in_hist;
  while ((in_hist = read_bytes(infile, buf, 1)) > 0) { // reading through files
    write_bytes(temp_num, buf, 1); // adding to temporary file
    for (uint8_t i = 0; i < in_hist; i++) {
      if (histogram[buf[i]] ==
          0) {           // if the histogram doesn't keep track of char already
        characters += 1; // incrementing characters
      }
      histogram[buf[i]] += 1; // adding to histogram
    }
  }

  characters += 2; // for 0x00 and 0x01

  infile = temp_num; // setting temporary file to the infile
  lseek(infile, 0, SEEK_SET);

  Node *tree = build_tree(histogram);
  Code code[ALPHABET] = {0}; // making code table
  build_codes(tree, code);   // building code table

  // header struct from header.h
  struct stat perm;
  fstat(infile, &perm);
  fchmod(outfile, perm.st_mode); // set permission to outfile

  Header header;
  header.magic = MAGIC;
  header.permissions = perm.st_mode;
  header.tree_size = ((3 * characters) - 1);
  header.file_size = perm.st_size;

  bytes_written = 0; // so that teh temp file doesn't mess with stats

  write_bytes(outfile, (uint8_t *)&header,
              sizeof(Header)); // write bytes to outfile
  dump_tree(outfile, tree);    // dumping the tree to the oufile

  // Starting at the beginning of infile, write the corresponding code for each
  // symbol to outfile
  lseek(infile, 0, SEEK_SET); // reset pointer to input

  while ((in_hist = read_bytes(infile, buf, 1)) >
         0) { // read the bytes from infile
    for (uint8_t i = 0; i < in_hist; i++) {
      write_code(outfile, &code[buf[i]]); // write code to output
    }
  }

  flush_codes(outfile); // flush out the code

  // printTree(tree, 0);

  double num = bytes_written;
  double div = (num / perm.st_size);
  double space_saved = 100 * (1 - (div));
  if (verbose == 1) { // printing stats
    fprintf(stderr, "Uncompressed file size: %lu bytes\n", perm.st_size);
    fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_written);
    fprintf(stderr, "Space saving: %.2lf%%\n", space_saved);
  }

  delete_tree(&tree); // deleting tree
  fclose(temp);       // closing temp file

  if (bol_one == true) { // closing infile
    close(infile);
  }
  if (bol_two == true) { // closing outfile
    close(outfile);
  }

  return 0;
}
