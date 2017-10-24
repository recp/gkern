/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "rn_prim.h"

void
gkRenderPrim(GkScene     * __restrict scene,
             GkPrimitive * __restrict prim) {
  if (prim->flags & GK_DRAW_ELEMENTS)
    glDrawElements(prim->mode,
                   prim->count,
                   GL_UNSIGNED_INT, /* TODO: ? */
                   NULL);
  else if (prim->flags & GK_DRAW_ARRAYS)
    glDrawArrays(prim->mode, 0, prim->count);
}