/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk-node.h"

void
gkMakeNodeMatrix(GkNode * __restrict node) {
  GkMatrix *mat;

  if (node->matrix)
    return;

  mat              = malloc(sizeof(*mat));
  mat->cmatIsValid = 0;
  mat->refc        = 1;
  mat->fmat        = NULL;
  node->matrix     = mat;
  node->flags     |= GK_HAVE_MATRIX;
}
