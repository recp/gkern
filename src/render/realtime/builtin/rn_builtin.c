/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../../include/gk/prims/builtin-prim.h"

#include "rn_builtin.h"
#include "rn_prim.h"

void
gkRenderBuiltinPrim(GkScene   *scene,
                    GkPrimType primType) {
  GkPrimitive *prim;
  prim = gkGetBuiltinPrim(primType);

  glBindVertexArray(prim->vao);
  gkRenderPrim(scene, prim);
}
