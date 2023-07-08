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

  // read the magic number to see if its BEEFD00D  32 bits
  uint32_t num_bytes = 0;
  uint8_t beefdod[4] = {0xbe, 0xef, 0xd0, 0x0d};
  uint8_t buffer[BLOCK];
  num_bytes = read_bytes(input_file, buffer, 4);
  if (num_bytes != 4) {
    return 1;
  }
  for (int i = 0; i < 4; i++) {
    if (buffer[3 - i] != beefdod[i]) {
      printf("incompatible file type\n");
      return 1;
    }
  }

  // read the file permissions (16 bits) and use fchmod() to set it
  num_bytes = 0;
  // uint16_t perm_cat = 0;
  num_bytes = read_bytes(input_file, buffer, 2);
  // perm_cat += ((uint16_t) buffer[1]) << 8;
  // perm_cat += buffer[0];

  // read the tree size
  num_bytes = 0;
  uint16_t t_size = 0;
  num_bytes = read_bytes(input_file, buffer, 2);
  t_size += ((uint16_t)buffer[0]);
  t_size += buffer[1] << 8;
  // get file size
  num_bytes = 0;
  uint64_t f_size = 0;
  num_bytes = read_bytes(input_file, buffer, 8);
  // you gotta take into account if its too small
  uint8_t stored = 0;
  for (int i = 7; i >= 0; i--) {
    if (buffer[i] != 0) {
      f_size = f_size << stored * 8;
      stored = 0;
      f_size += buffer[i];
    }
    stored++;
  }

  // ACTUAL DECODING

  // get a buffer with the entire tree dump
  num_bytes = read_bytes(input_file, buffer, t_size);

  Node *redwood = rebuild_tree(t_size, buffer);

  Node *curr_pos = redwood;

  // open the out file
  int oput = 0;
  if (member_set(OUTFILE, commands)) {
    oput = open(outfile, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
  } else {
    oput = STDOUT_FILENO;
  }
  uint64_t translated = 0;
  while (translated < f_size) {
    uint8_t curr_bit = 0;
    read_bit(input_file, &curr_bit);
    if (curr_bit == 0) {
      curr_pos = curr_pos->left;
    } else {
      curr_pos = curr_pos->right;
    }

    if (curr_pos->left == NULL) {
      write_bytes(oput, &curr_pos->symbol, 1);
      translated++;
      curr_pos = redwood;
    }
  }
  // no need to flush since you know it has to be a full amount of bytes
  close(oput);
  curr_pos = NULL;

  if (member_set(COMPSTATS, commands)) {
    fprintf(stderr, "\n");
    fprintf(stderr, "Uncompressed file size:  %lu\n", bytes_written);
    fprintf(stderr, "Compressed file size: %lu\n", bytes_read);
    fprintf(stderr, "Space saving: %.2f%%\n",
            100 * (1 - ((float)bytes_read / (float)bytes_written)));
  }

  delete_tree(&redwood);
  close(oput);
  close(input_file);
  return 0;
}
