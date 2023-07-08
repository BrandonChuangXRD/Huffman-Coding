#include <stdio.h>

#include "helpprints.h"

void helpprintsencode(void) {
  printf("SYNOPSIS\n");
  printf("  Encodes files with the Huffman coding algorithm\n");
  printf("USAGE\n");
  printf("./encode [-h] [-i infile] [-o outfile] [-v]\n");
  printf("OPTIONS\n");
  printf("-h: prints out help\n");
  printf("-i: records the next input as the name of the input file\n");
  printf("-o: records the next input as the name of the output file\n");
  printf("-v: prints out the compression statistics\n");
}

void helpprintsdecode(void) {
  printf("SYNOPSIS\n");
  printf("  Decodes files with the Huffman coding algorithm\n");
  printf("USAGE\n");
  printf("./decode [-h] [-i infile] [-o outfile] [-v]\n");
  printf("OPTIONS\n");
  printf("-h: prints out help\n");
  printf("-i: records the next input as the name of the input file\n");
  printf("-o: records the next input as the name of the output file\n");
  printf("-v: prints out the compression statistics\n");
}
