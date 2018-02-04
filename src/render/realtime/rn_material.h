/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef material_h
#define material_h

void
gkApplyMaterial(GkScene    * __restrict scene,
                GkPrimInst * __restrict primInst);

void
gkApplyMaterials(GkScene    * __restrict scene,
                 GkPrimInst * __restrict primInst);

#endif /* material_h */
