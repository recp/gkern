/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/prims/builtin-prim.h"
#include "../../include/gk/gk.h"
#include <limits.h>

GK_EXPORT
GkPrimitive*
gkGetBuiltinPrim(GkPrimType primType) {
  switch (primType) {
    case GK_PRIM_TEXQUAD:
      return gkPrimGetTexQuad();
    default:
      return NULL;
  }
}
