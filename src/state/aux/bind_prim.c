/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../../include/gk/gpu_state.h"

GK_EXPORT
void
gkBindPrimitive(GkPrimitive * __restrict prim) {
  glBindVertexArray(prim->vao);
}
