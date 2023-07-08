#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "code.h"
#include "defines.h"
#include "header.h"
#include "helpprints.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "set.h"
#include "stack.h"

#define OPTIONS "hi:o:v"
#define COMPSTATS 0
#define INFILE 1
#define OUTFILE 2

int main(int argc, char **argv) {
  // Interpret the inputs
  int opt = 0;
  Set commands = empty_set();
  char *infile = " ";
  char *outfile = " ";
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      helpprintsencode();
      return 0;
    case 'i':
      commands = insert_set(INFILE, commands);
      infile = optarg;
      break;
    case 'o':
      commands = insert_set(OUTFILE, commands);
      outfile = optarg;
      break;
    case 'v':
      commands = insert_set(COMPSTATS, commands);
      break;
    }
  }

  // fill out histogram
  uint64_t histogram[ALPHABET];
  // set all values in histogram to zero
  for (uint32_t i = 0; i < ALPHABET; i++) {
    histogram[i] = 0;
  }
  // set 0x00 and 0xFF to 1 manually
  histogram[0] = 1;
  histogram[255] = 1;

  // open file
  int input_file = 0;
  if (member_set(INFILE, commands)) {
    input_file = open(infile, O_RDONLY, 0);
    if (input_file == -1) {
      printf("file does not exist.\n");
      return 1;
    }
  } else {
    // if member set was not called, do this
    input_file = STDIN_FILENO;
    if (input_file == -1) {
      printf("file does not exist.\n");
      return 1;
    }
  }

  // read and input into histogram
  int num_bytes_read = 0;
  uint8_t read_block[BLOCK];

  while (true) {
    num_bytes_read = read_bytes(input_file, read_block, BLOCK);
    // if read actually read something
    if (num_bytes_read > 0) {
      for (int i = 0; i < num_bytes_read; i++) {
        histogram[(uint8_t)read_block[i]]++;
      }
    } else {
      // end the loop if it doesn't read anything
      break;
    }
  }

  // make the tree
  Node *redwood = build_tree(histogram);

  // record the codes
  Code newreps[ALPHABET];
  build_codes(redwood, newreps);

  // time to work on the actual output

  // write BEEFD00D (32 bits)
  // write permissions(16 bits)
  // write tree size(16 bits)
  // write file size(64 bits)
  // HEADER TIME
  // fstat() to find stuff
  struct stat instats;
  fstat(input_file, &instats);

  Header head;
  head.magic = MAGIC;
  head.permissions = instats.st_mode;
  uint16_t num_codes = 0;
  for (int i = 0; i < ALPHABET; i++) {
    if (histogram[i] != 0) {
      num_codes++;
    }
  }
  head.tree_size = (num_codes * 3) - 1;

  head.file_size = instats.st_size;

  // open the file
  int oput = 0;
  if (member_set(OUTFILE, commands)) {
    oput = open(outfile, O_WRONLY | O_CREAT, 0);
  } else {
    oput = STDOUT_FILENO;
  }
  fchmod(oput, instats.st_mode);
  // write the header
  write(oput, &head, sizeof(Header));

  // dump the tree
  dump_tree(oput, redwood);

  // print the encoded message
  lseek(input_file, 0, SEEK_SET); // reset the pointer back to the beginning
  num_bytes_read = 0;             // reset this too
  while (true) {
    num_bytes_read = read_bytes(input_file, read_block, BLOCK);
    // if read actually read something
    if (num_bytes_read > 0) {
      for (int i = 0; i < num_bytes_read; i++) {
        write_code(oput, &newreps[read_block[i]]);
      }
    } else {
      // end the loop if it doesn't read anything
      break;
    }
  }
  flush_codes(oput);
  close(oput);
  // write the verbose part
  if (member_set(COMPSTATS, commands)) {
    fprintf(stderr, "\n");
    fprintf(stderr, "Uncompressed file size:  %lu\n", instats.st_size);
    fprintf(stderr, "Compressed file size: %lu\n",
            bytes_written + sizeof(head));
    fprintf(
        stderr, "Space saving: %.2f%%\n",
        100 * (1 - ((float)(bytes_written + sizeof(head)) / instats.st_size)));
  }
  // this wont work until build tree does
  delete_tree(&redwood);
  close(input_file);
  return 0;
}
