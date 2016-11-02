/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

void
gkRenderNode(GkNode * node) {
  while (node) {
    if (node->model)
      gkRenderModel(node->model);
    else if (node->instance)
      gkRenderInstance(node->instance);

    if (node->chld)
      gkRenderNode(node->chld);

    node = node->next;
  }
}
