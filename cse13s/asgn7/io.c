// io.c
#include "io.h"
#include "code.h"
#include <stdio.h>  // for printf
#include <unistd.h> // for reading/writing

uint64_t bytes_read = 0;    // stat
uint64_t bytes_written = 0; // stat
static uint8_t array_buffer[BLOCK];
static uint32_t index;

// pseudocode from tutor lev
int read_bytes(int infile, uint8_t *buf, int nbytes) {
  int curr_read = read(infile, buf, nbytes);
  int total_read = curr_read;
  while (curr_read > 0 && total_read < nbytes) {
    curr_read = read(infile, buf + total_read, nbytes - total_read);
    total_read += curr_read;
  }
  bytes_read += total_read; // incrementing stats
  return total_read;
}

// pseudocode from tutor lev
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
  int curr_write = write(outfile, buf, nbytes);
  int total_write = curr_write;
  while (curr_write > 0 && total_write < nbytes) {
    curr_write = write(outfile, buf + total_write, nbytes - total_write);
    total_write += curr_write;
  }
  bytes_written += total_write; // incrementing stats
  return total_write;
}

// pseudocode from tutor lev
bool read_bit(int infile, uint8_t *bit) {
  static uint8_t buffer[BLOCK];
  static uint32_t size;

  if (index == 0) {
    size = 8 * read_bytes(infile, buffer, BLOCK);
  }

  uint8_t one = 1; // bit masking
  *bit = one & (buffer[index / 8] >>
                index % 8); // finding the bit position and getting state of bit
  index = (index + 1) % (BLOCK * 8); // adjusting index

  if (index < size) {
    return true;
  }

  return false;
}

// explained by tutor lev
void flush_codes(int outfile) {
  int flush = index / 8;

  if ((1 << (index % 8)) - 1) {
    array_buffer[flush++] &= (1 << (index % 8)) - 1;
  }

  write_bytes(outfile, array_buffer, flush);
}

void write_code(int outfile, Code *c) {
  // array_buffer[BLOCK]
  // index
  uint32_t total_size = code_size(c); // getting size
  for (uint8_t i = 0; i < total_size; i++) {

    uint8_t each_bit = code_get_bit(c, i);

    if (each_bit == 1) {
      array_buffer[index / 8] |= (1 << (index % 8)); // setting bit
    } else {
      array_buffer[index / 8] &= ~(1 << (index % 8)); // clearing bit
    }
    index++; // incrementing index

    if (index == BLOCK * 8) {
      write_bytes(outfile, array_buffer, BLOCK);
      index = 0;
    }
  }
}
