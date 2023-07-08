#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "code.h"
#include "defines.h"

Code code_init(void) {
  Code c = {.top = 0, .bits = {0}};
  return c;
}

uint32_t code_size(Code *c) { return c->top; }

bool code_empty(Code *c) { return c->top == 0; }

bool code_full(Code *c) { return c->top == ALPHABET; }

bool code_set_bit(Code *c, uint32_t i) {
  // check if i is out of bounds
  if (i >= c->top) {
    return false;
  }

  uint32_t pos = i / 8;
  uint8_t tween = i % 8;

  c->bits[pos] = c->bits[pos] | (1 << tween);
  return true;
}

bool code_clr_bit(Code *c, uint32_t i) {
  // check if i is out of bounds
  if (i >= c->top) {
    return false;
  }
  // check range of i against ALPHABET from define.h (?)
  uint32_t pos = i / 8;
  uint8_t tween = i % 8;

  c->bits[pos] = c->bits[pos] & ~(1 << tween);
  return true;
}

bool code_get_bit(Code *c, uint32_t i) {
  // check if i is out of bounds
  if (i >= c->top) {
    return false;
  }

  uint32_t pos = i / 8;
  uint8_t tween = i % 8;

  return (c->bits[pos] >> tween) & 1;
}

bool code_push_bit(Code *c, uint8_t bit) {
  // check if top is within MAX_CODE_SIZE
  if (c->top == ALPHABET) {
    return false;
  }
  c->top++;
  // push the bit and update top
  // clear bit if the bit is 0
  if (bit == 0) {
    code_clr_bit(c, c->top - 1);
    return true;
  }

  // set bit if the bit is 1
  code_set_bit(c, c->top - 1);
  return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
  // check the "stack" is empty
  if (c->top == 0) {
    return false;
  }
  // pop the bit
  *bit = code_get_bit(c, c->top - 1);
  c->top--;
  return true;
}

void code_print(Code *c) {
  printf("CODE PRINTOUT:\n");
  uint32_t lim = c->top / 8;
  uint32_t tween = c->top % 8;
  printf("TOP: %u\n", c->top);
  if (c->top % 8 != 0) {
    lim++;
  }
  for (uint32_t i = 0; i < lim; i++) {
    for (int j = tween - 1; j >= 0; j--) {
      printf("%hhu", code_get_bit(c, (i * 8) + j));
    }
    printf(" ");
  }
  printf("\n");
}
