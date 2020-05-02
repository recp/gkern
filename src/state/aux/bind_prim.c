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
  if (!prim)
    return;

  glBindVertexArray(prim->vao);
}

GK_EXPORT
void
gkBindPrimInst(GkPrimInst * __restrict primInst) {
  GkPrimitive *prim;
  
  if (!primInst || !(prim = primInst->prim))
    return;

  glBindVertexArray(prim->vao);
}
