/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk-node.h"

void
gkMakeNodeTransform(GkNode * __restrict node) {
  GkTransform *tr;

  if (node->trans)
    return;

  tr           = malloc(sizeof(*tr));
  tr->flags    = 0;
  tr->refc     = 1;
  tr->item     = NULL;
  tr->ftr      = NULL;
  node->trans  = tr;
  node->flags |= GK_HAVE_MATRIX;
}
