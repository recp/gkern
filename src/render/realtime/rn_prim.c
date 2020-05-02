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

#include "../../common.h"
#include "rn_prim.h"
#include "rn_material.h"
#include "../../../include/gk/prims/cube.h"

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

void
gkRenderPrimInst(GkScene    * __restrict scene,
                 GkPrimInst * __restrict primInst) {
  GkPrimitive *prim;
  
  prim = primInst->prim;

  glBindVertexArray(prim->vao);
  gkApplyMaterial(scene, primInst);

  if ((scene->flags & GK_SCENEF_DRAW_PRIM_BBOX))
    gkDrawBBox(scene,
               primInst->bbox,
               primInst->trans->world);
}
