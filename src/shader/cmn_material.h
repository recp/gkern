/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef cmn_material_h
#define cmn_material_h

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"

size_t
gkShaderNameFor(GkScene     * __restrict scene,
                GkLight     * __restrict light,
                GkPrimitive * __restrict prim,
                GkMaterial  * __restrict mat,
                char        * __restrict nameBuff);

void
gkShaderFlagsFor(GkScene     * __restrict scene,
                 GkLight     * __restrict light,
                 GkPrimitive * __restrict prim,
                 GkMaterial  * __restrict mat,
                 char       ** __restrict vertFlags,
                 char       ** __restrict fragFlags);

GkShader*
gkShadersFor(GkScene     * __restrict scene,
             GkLight     * __restrict light,
             GkPrimitive * __restrict prim,
             GkMaterial  * __restrict mat);

GkProgram*
gkGetOrCreatProgForCmnMat(GkScene     * __restrict scene,
                          GkLight     * __restrict light,
                          GkPrimitive * __restrict prim,
                          GkMaterial  * __restrict mat);

#endif /* cmn_material_h */
