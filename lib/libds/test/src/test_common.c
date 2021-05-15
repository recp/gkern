/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "test_common.h"
#include <time.h>

void
rand_str(char *dest, size_t length) {
  char charset[] = "0123456789"
                   "abcdefghijklmnopqrstuvwxyz"
                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  while (length-- > 0) {
    size_t index;
    index   = (double)rand() / RAND_MAX * (sizeof(charset) - 1);
    *dest++ = charset[index];
  }
  *dest = '\0';
}
