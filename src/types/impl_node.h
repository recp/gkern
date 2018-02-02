/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef impl_node_h
#define impl_node_h

#include "../../include/gk/gk.h"

typedef struct GkNodePage {
  struct GkNodePage *next;
  uint32_t           size;
  uint32_t           count;
  GkNode             nodes[];
} GkNodePage;

#endif /* impl_node_h */
