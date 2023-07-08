#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "code.h"
#include "defines.h"
#include "io.h"

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
  // if the amount of data wanted is nothing then return nothing
  if (nbytes == 0) {
    return 0;
  }
  // loop and store values in the buffer until nbytes is stored
  int curr_bytes_read = 0;
  while (true) {
    int new_read =
        read(infile, buf + curr_bytes_read, nbytes - curr_bytes_read);
    // if read() returns -1, that means that nothing happened or something went
    // wrong
    if (new_read <= 0) {
      return curr_bytes_read;
    }
    // if the amount of read values is equal to the bytes requested
    curr_bytes_read += new_read;
    if (curr_bytes_read == nbytes) {
      bytes_read += curr_bytes_read;
      return curr_bytes_read;
    }
  }
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
  // basically identical to read_bytes
  if (nbytes == 0) {
    return 0;
  }

  int curr_bytes_written = 0;

  while (true) {
    int new_written =
        write(outfile, buf + curr_bytes_written, nbytes - curr_bytes_written);
    if (new_written <= 0) {
      return curr_bytes_written;
    }
    curr_bytes_written += new_written;
    if (curr_bytes_written == nbytes) {
      bytes_written += curr_bytes_written;
      return curr_bytes_written;
    }
  }
}

bool read_bit(int infile, uint8_t *bit) {
  static int readtop = 0;
  static int read_left_off = 0;
  static uint8_t readbuffer[BLOCK];
  // refill the buffer if theres nothing in it anymore
  read_left_off++;
  if (readtop == 0 || read_left_off == readtop) {
    // multiplied by 8 to get the amount of bits rather than bytes
    readtop = read_bytes(infile, readbuffer, BLOCK) * 8;
    if (readtop == -1) {
      return false;
    }
    read_left_off = 0;
  }
  // read from the bottom till readtop
  uint16_t kb_truncated = read_left_off / 8;
  uint8_t bits_remaining = read_left_off % 8;
  // set the *bit pointer to the value
  uint8_t byte_value = readbuffer[kb_truncated];
  *bit = (byte_value >> bits_remaining) &
         1; // CHANGED THIS, IT USED TO BE 7- BITS_REMAINING
  return true;
}

// fills up writebuffer till it has BLOCK (4096) bytes stored
// then flush will be used, even if its less than 4kb for the remaining
// bytes/bits that don't fill up the writebuffer
static uint8_t writebuffer[BLOCK];
static uint64_t writetop;

void write_code(int outfile, Code *c) {
  uint64_t curr_write_bits = 0;

  while (curr_write_bits != code_size(c)) {
    uint32_t bytes_truncated = writetop / 8;
    uint8_t bits_remaining = writetop % 8;

    // write the bits in
    if (code_get_bit(c, curr_write_bits) == 0) {
      writebuffer[bytes_truncated] =
          writebuffer[bytes_truncated] & ~(1 << bits_remaining);
    } else {
      writebuffer[bytes_truncated] =
          writebuffer[bytes_truncated] | (1 << bits_remaining);
    }
    writetop++;
    curr_write_bits++;

    // if the block is full, flush and reset the top to 0
    if (writetop == BLOCK) {
      // write the stuff
      flush_codes(outfile);
      writetop = 0;
    }
  }
}

void flush_codes(int outfile) {
  // set the remaining bits to 0
  while (writetop % 8 != 0) {
    uint32_t bytes_truncated = writetop / 8;
    uint8_t bits_remaining = writetop % 8;

    writebuffer[bytes_truncated] =
        writebuffer[bytes_truncated] & ~(1 << bits_remaining);
    writetop++;
  }
  write_bytes(outfile, writebuffer, writetop / 8);
}
