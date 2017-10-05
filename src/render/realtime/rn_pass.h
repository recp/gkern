/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef pass_h
#define pass_h

void
gkRenderPrim(GkScene     * __restrict scene,
             GkPrimitive * __restrict prim);

void
gkRenderPass(GkScene     * __restrict scene,
             GkModelInst * __restrict modelInst,
             GkPrimitive * __restrict prim,
             GkPass      * __restrict pass);

#endif /* pass_h */
