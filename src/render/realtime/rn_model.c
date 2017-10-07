/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"
#include "../../../include/gk/prims/cube.h"
#include "../../default/gk_def_effect.h"
#include "../../default/ak_def_light.h"
#include "../../gk_matrix.h"

#include "rn_light.h"
#include "rn_material.h"

void
gkPrepModel(GkScene     *scene,
            GkModelInst *modelInst,
            GkTransform *ptr) {
  GkTransform *tr;
  uint32_t     updt;

  tr = modelInst->trans;

  if (!tr)
    modelInst->trans = tr = ptr;

  updt = !((ptr->flags & tr->flags) & GK_TRANSF_WORLD_ISVALID);

  if (updt) {
    if (ptr != tr) {
      if (!GK_FLG(tr->flags, GK_TRANSF_LOCAL_ISVALID))
        gkTransformCombine(tr);

      glm_mat4_mul(ptr->world,
                   tr->local,
                   tr->world);
      tr->flags &= ~GK_TRANSF_WORLD_ISVALID;
    }

    gkCalcFinalMat(scene, tr);
  } else if(scene->flags & GK_SCENEF_UPDT_VIEWPROJ) {
    gkCalcFinalMat(scene, tr);
  }

  if(updt && tr != ptr)
    tr->flags |= GK_TRANSF_WORLD_ISVALID;
}

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst,
              GkTransform *ptr) {
  GkModel     *model;
  GkPrimitive *primi;
  GkTransform *tr;
  GkMaterial  *modelMaterial;

  model = modelInst->model;
  tr    = modelInst->trans;

  /* model's material */
  modelMaterial = NULL;
  if (!modelInst->prims
      || modelInst->material
      || modelInst->model->material) {
    modelMaterial = modelInst->material;
    if (!modelMaterial)
      modelMaterial = modelInst->model->material;
  }

  /* pre events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, false);

  /* render */
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);

    gkApplyMaterials(scene,
                     primi,
                     modelInst,
                     modelMaterial);

    primi = primi->next;
  }

  /* reset the state */
  glBindVertexArray(0);
  /* TODO: unbind all textures */

  /* post events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, true);

  if ((model->flags & GK_MODEL_FLAGS_DRAW_BBOX)
      && model->bbox)
    gkDrawBBox(scene,
               tr->world,
               model->bbox->min,
               model->bbox->max);
}
