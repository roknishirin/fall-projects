// decode.c

#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#include <ctype.h>     // for printTree
#include <fcntl.h>     // for open
#include <inttypes.h>  // for printTree
#include <stdio.h>     // for printf
#include <string.h>    // for strlen
#include <sys/stat.h>  // for fstat
#include <sys/types.h> // for fstat
#include <unistd.h>    // for stdin_fileno and getopt
#include <unistd.h>    // for fstat

#define OPTIONS "hi:o:v"
/*
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

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      fprintf(stderr,
              "SYNOPSIS\n"
              "  A Huffman decoder.\n"
              "  Decompresses a file using the Huffman coding algorithm.\n\n"

              "USAGE\n"
              "  ./decode [-h] [-i infile] [-o outfile]\n\n"

              "OPTIONS\n"
              "  -h             Program usage and help.\n"
              "  -v             Print compression statistics.\n"
              "  -i infile      Input file to decompress.\n"
              "  -o outfile     Output of decompressed data.\n");
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
              "  A Huffman decoder.\n"
              "  Decompresses a file using the Huffman coding algorithm.\n\n"

              "USAGE\n"
              "  ./decode [-h] [-i infile] [-o outfile]\n\n"

              "OPTIONS\n"
              "  -h             Program usage and help.\n"
              "  -v             Print compression statistics.\n"
              "  -i infile      Input file to decompress.\n"
              "  -o outfile     Output of decompressed data.\n");
      return -1;
    }
  }

  Header head;
  read_bytes(infile, (uint8_t *)&head,
             sizeof(Header)); // reading bytes from infile

  struct stat perm;
  fstat(infile, &perm);
  fchmod(outfile, head.permissions); // permissions for outfile

  // printf("my magic: %d their magic: %d\n", head.magic, MAGIC);

  if (head.magic != MAGIC) { // checking for if the magic number matches
    fprintf(stderr, "magic value is incorrect\n");
    return -1;
  }

  uint8_t
      sz_tree[head.tree_size]; // make the tree the size of the header tree size
  read_bytes(infile, sz_tree, head.tree_size); // reading the dumped tree

  Node *tree = rebuild_tree(head.tree_size,
                            sz_tree); // rebuild the tree from dump tree to
                                      // input file printTree(tree, 0);

  Node *node = tree; // swapping
  Node *root = tree; // swapping

  uint64_t counter = 0;
  uint8_t bit;
  bool cont = true; // breakout bool

  // printTree(tree, 0);

  while ((counter < head.file_size) &&
         cont) { // while the file is still being read and there is something to
                 // read
    cont = read_bit(infile, &bit); // return false if no more to read
    if (node->right == NULL && node->left == NULL) { // if at a leaf
      write_bytes(outfile, &(node->symbol), 1); // write the bytes into outfile
      node = root;                              // swap roots
      counter += 1;                             // increment counter
    }
    if (bit == 1) { // go down right path
      node = node->right;
    } else if (bit == 0) { // go down left path
      node = node->left;
    }
  }

  // printTree(tree, 0);

  double num = bytes_read;
  double div = (num / bytes_written);
  double space_saved = 100 * (1 - (div));
  if (verbose == 1) {
    fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_read);
    fprintf(stderr, "Decompressed file size: %lu bytes\n", bytes_written);
    fprintf(stderr, "Space saving:  %.2lf%%\n", space_saved);
  }

  delete_tree(&tree); // deleting tree

  if (bol_one == true) { // closing infile
    close(infile);
  }
  if (bol_two == true) { // closing outfile
    close(outfile);
  }

  return 0;
}
