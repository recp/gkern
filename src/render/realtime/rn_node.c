/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../gk_matrix.h"

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
        gkRenderModel(scene,
                      modelInst,
                      tr);
        modelInst = modelInst->next;
      } while (modelInst);
    }

    if (node->chld)
      gkRenderNode(scene,
                   node->chld,
                   tr);

    if (node->nodeInst)
      gkRenderNode(scene,
                   node->nodeInst,
                   tr);

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

    tr = node->trans;

    if (!tr)
      node->trans = tr = ptr;

    updt = !((ptr->flags & tr->flags) & GK_TRANSF_WORLD_ISVALID);

    if (updt && ptr != tr) {
      /* transform may be animated */
      if (!GK_FLG(tr->flags, GK_TRANSF_LOCAL_ISVALID))
        gkTransformCombine(tr);

      glm_mat4_mul(ptr->world,
                   tr->local,
                   tr->world);
      tr->flags &= ~GK_TRANSF_WORLD_ISVALID;
    }

    if (node->light
        && (!(GK_FLG(tr->flags, GK_TRANSF_WORLD_ISVALID))
            || !(GK_FLG(tr->flags, GK_TRANSF_FMAT_MV))
            || GK_FLG(scene->flags, GK_SCENEF_UPDT_VIEW))) {
      gkCalcViewMat(scene, tr);
    }

    if (node->model) {
      GkModelInst *modelInst;
      modelInst = node->model;
      do {
        gkPrepModel(scene,
                    modelInst,
                    tr);
        modelInst = modelInst->next;
      } while (modelInst);
    }

    if (node->chld)
      gkPrepNode(scene, node->chld, tr);

    if (node->nodeInst)
      gkPrepNode(scene, node->nodeInst, tr);

    if (updt && tr != ptr)
      tr->flags |= GK_TRANSF_WORLD_ISVALID;
    
    node = node->next;
  }
}
