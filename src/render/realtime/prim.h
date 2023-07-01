/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

GK_INLINE
void
gkRenderPrims(GkScene      * __restrict scene,
              GkRenderList * __restrict rnlist) {
  GkPrimInst **prims;
  size_t       i, primc;

  primc     = rnlist->count;
  prims     = rnlist->items;

  for (i = 0; i < primc; i++) {
    if (!scene->renderPrimFunc)
      gkRenderPrimInst(scene, prims[i]);
    else
      scene->renderPrimFunc(scene, prims[i]);
  }
}

#endif /* rn_prim_h */
