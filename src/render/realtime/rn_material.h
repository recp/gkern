/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef material_h
#define material_h

void
gkApplyMaterial(GkScene     * __restrict scene,
                GkModelInst * __restrict modelInst,
                GkPrimitive * __restrict prim,
                GkMaterial  * __restrict material);

void
gkApplyMaterials(GkScene     * __restrict scene,
                 GkPrimitive * __restrict prim,
                 GkModelInst * __restrict modelInst,
                 GkMaterial  * __restrict modelMaterial);

#endif /* material_h */
