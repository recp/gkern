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

void
gkPrepMaterial(GkScene     *scene,
               GkModelInst *modelInst) {
  GkModel     *model;
  GkPrimitive *primi;
  GkMaterial  *modelMaterial;

  model = modelInst->model;
  /*
   make sure model instance has a material
   (to avoid access model multiple times)
   */
  modelMaterial = NULL;
  if (!modelInst->material) {
    /* model doesn't has a material set default material */
    if (!(modelInst->activeMaterial = modelMaterial = model->material))
      modelInst->activeMaterial = modelMaterial = gk_def_material();
  }
  
  /* make sure every primitive has a material */
  primi = model->prim;
  while (primi) {
    if (!(primi->activeMaterial = primi->material))
      primi->activeMaterial = modelMaterial;
    primi = primi->next;
  }
}

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
  GkTransform *tr;

  model = modelInst->model;
  tr    = modelInst->trans;

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
