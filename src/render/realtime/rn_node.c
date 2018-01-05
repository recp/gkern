/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"
#include "../../types/impl_transform.h"

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
      /* transform may be animated */
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
