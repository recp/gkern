/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"
#include "../../../include/gk/prims/cube.h"
#include "../../default/def_effect.h"
#include "../../default/def_light.h"
#include "../../matrix.h"

#include "rn_light.h"
#include "rn_material.h"
#include "rn_pass.h"

void
gkPrepModel(GkScene     *scene,
            GkModelInst *modelInst,
            GkTransform *ptr) {
  GkTransform *tr;
  uint32_t     updt;

  if (!(tr = modelInst->trans))
    modelInst->trans = tr = ptr;

  updt = !((ptr->flags & tr->flags) & GK_TRANSF_WORLD_ISVALID);
  if (updt) {
    if (ptr != tr) {
      if (!GK_FLG(tr->flags, GK_TRANSF_LOCAL_ISVALID))
        gkTransformCombine(tr);

      glm_mat4_mul(ptr->world, tr->local, tr->world);
      tr->flags &= ~GK_TRANSF_WORLD_ISVALID;
    }

    gkCalcFinalMat(scene, tr);
  } else if(scene->flags & GK_SCENEF_UPDT_VIEWPROJ) {
    gkCalcFinalMat(scene, tr);
  }

  if(updt && tr != ptr)
    tr->flags |= GK_TRANSF_WORLD_ISVALID;
  
  if (!modelInst->activeMaterial)
    return;

  gkPrepMaterial(scene, modelInst);
}

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst,
              GkTransform *ptr) {
  GkModel     *model;
  GkPrimitive *primi;

  model = modelInst->model;

  /* pre events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, false);

  /* render */
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);

    gkApplyMaterials(scene, modelInst, primi);

    primi = primi->next;
  }

  /* reset the state */
  glBindVertexArray(0);

  if ((model->flags & GK_MODEL_FLAGS_DRAW_BBOX) && model->bbox)
    gkDrawBBox(scene,
               modelInst->trans->world,
               model->bbox->min,
               model->bbox->max);

  /* post events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, true);
}

void
gkRnModelNoMatOPass(GkScene     *scene,
                    GkModelInst *modelInst,
                    GkTransform *ptr) {
  GkModel     *model;
  GkPrimitive *primi;
  
  model = modelInst->model;

  /* render */
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);
    gkRenderPass(scene,
                 modelInst,
                 primi,
                 NULL,
                 scene->_priv.overridePass);
    primi = primi->next;
  }

  /* reset the state */
  glBindVertexArray(0);
}
