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
#include "../../../include/gk/gk.h"
#include "../../types/impl_transform.h"

/*

void
gkRenderNode(GkScene     *scene,
             GkNode      *node,
             GkTransform *ptr) {
  while (node) {
    GkTransform *tr;

    tr = node->trans;
    if (node->model) {
      GkModelInst *modelInst;
      modelInst = node->model;
      do {
        if (!scene->renderModelFn)
          gkRenderModel(scene, modelInst);
        else
          scene->renderModelFn(scene, modelInst);
        modelInst = modelInst->next;
      } while (modelInst);
    }

    if (node->chld)
      gkRenderNode(scene, node->chld, tr);

    node = node->next;
  }
}

void
gkPrepNode(GkScene     *scene,
           GkNode      *node,
           GkTransform *ptr) {
  while (node) {
    GkTransform *tr;
    uint32_t     updt;

    if (!(tr = node->trans))
      node->trans = tr = ptr;

    updt = !((ptr->flags & tr->flags) & GK_TRANSF_WORLD_ISVALID);

    if (updt && ptr != tr) {
      / * transform may be animated * /
      if (!GK_FLG(tr->flags, GK_TRANSF_LOCAL_ISVALID))
        gkTransformCombine(tr);

      glm_mat4_mul(ptr->world, tr->local, tr->world);
      tr->flags &= ~GK_TRANSF_WORLD_ISVALID;

      if (node->bbox)
        gkTransformAABB(tr, node->bbox);
    }

    if (node->light
        && (!(GK_FLG(tr->flags, GK_TRANSF_WORLD_ISVALID))
            || !(GK_FLG(tr->flags, GK_TRANSF_FMAT_MV))
            || GK_FLG(gkActiveCamera(scene)->flags, GK_UPDT_VIEW))) {
      gkCalcViewTransf(scene, gkActiveCamera(scene), tr);
    }

    if (node->model) {
      GkModelInst *modelInst;
      modelInst = node->model;
      do {
        gkPrepModel(scene, modelInst, tr);
        modelInst = modelInst->next;
      } while (modelInst);
    }

    if (node->chld)
      gkPrepNode(scene, node->chld, tr);

    if (updt && tr != ptr)
      tr->flags |= GK_TRANSF_WORLD_ISVALID;

    node = node->next;
  }
}

*/
