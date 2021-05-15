/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef ds_src_common_h
#define ds_src_common_h

#include "../include/ds/common.h"
#include "../include/ds/allocator.h"
#include "../include/ds/util.h"

#include "default/default.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define swap(a, b)                                                            \
  do {                                                                        \
    typeof(a) t;                                                              \
    t = a;                                                                    \
    a = b;                                                                    \
    b = t;                                                                    \
  } while(0)

#define swap2(t, a, b)                                                        \
  do {                                                                        \
    t = a;                                                                    \
    a = b;                                                                    \
    b = t;                                                                    \
  } while(0)

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#endif /* ds_src_common_h */
