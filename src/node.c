/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "../include/gk/scene.h"
#include "../include/gk/node.h"
#include <ds/hash.h>

void
gkMakeNodeTransform(GkScene * __restrict scene,
                    GkNode  * __restrict node) {
  GkTransform *tr;

  if (node->trans)
    return;

  tr           = gkAllocTransform(scene);
  node->trans  = tr;
  node->flags |= GK_HAVE_MATRIX;
}
