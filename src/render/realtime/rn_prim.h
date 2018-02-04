/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef rn_prim_h
#define rn_prim_h

#include "../../../include/gk/gk.h"

void
gkRenderPrim(GkScene     * __restrict scene,
             GkPrimitive * __restrict prim);

void
gkRenderPrimInst(GkScene    * __restrict scene,
                 GkPrimInst * __restrict primInst);

#endif /* rn_prim_h */
