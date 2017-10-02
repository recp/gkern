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
gkShaderNameFor(GkTechnique * __restrict techn,
                char        * __restrict nameBuff,
                char        * __restrict prefix);

void
gkShaderFlagsFor(GkTechnique * __restrict techn,
                 char       ** __restrict vertFlags,
                 char       ** __restrict fragFlags);

GkShader*
gkShadersFor(GkTechnique * __restrict techn);

GkProgInfo*
gkGetOrCreatProgForCmnMat(GkMaterial *mat);

#endif /* cmn_material_h */
