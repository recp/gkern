/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk.h"
#include "../../../include/gk-material.h"
#include "../../../include/prims/gk-cube.h"
#include "../../default/gk_def_effect.h"
#include "../../gk_matrix.h"

void
gkPrepModel(GkScene     *scene,
            GkModelInst *modelInst,
            GkTransform *ptr,
            GkProgInfo  *pprog) {
  GkModel     *model;
  GkTransform *tr;
  GkProgInfo  *prog;
  uint32_t     updt;

  model = modelInst->model;
  tr    = modelInst->trans;
  prog  = model->pinfo;

  if (!tr)
    modelInst->trans = tr = ptr;

  updt = !((ptr->flags & tr->flags) & GK_TRANSF_WORLD_ISVALID);

  if (updt){
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

  if (!prog)
    model->pinfo = prog = pprog;

  if(updt && tr != ptr)
    tr->flags |= GK_TRANSF_WORLD_ISVALID;
}

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst,
              GkTransform *ptr,
              GkProgInfo  *pprog) {
  GkModel     *model;
  GkPrimitive *primi;
  GkTransform *tr;
  GkProgInfo  *prog;
  GkMaterial  *modelMaterial;

  model = modelInst->model;
  tr    = modelInst->trans;
  prog  = model->pinfo;

  gkUniformMatrix(modelInst);

  /* uniform lights for this program */
  if (prog->updtLights)
    gkUniformLights(scene, prog);

  /* model's material */
  modelMaterial = NULL;
  if (!modelInst->prims
      || modelInst->material
      || modelInst->model->material) {
    modelMaterial = modelInst->material;
    if (!modelMaterial)
      modelMaterial = modelInst->model->material;

    /* avoid uniform cached material if possible */
    if (prog->lastMaterial != modelMaterial
        || prog->updtMaterials)
      gkUniformMaterial(prog, modelMaterial);
  }

  /* pre events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, false);

  /* TODO: multi thread ? */
  /* each node, model may use different program/shaders */
  if (scene->currentProgram != model->pinfo->prog) {
    glUseProgram(model->pinfo->prog);
    scene->currentProgram = model->pinfo->prog;
  }

  /* render */
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);

    /* instance primitive specific effects */
    if (modelInst->prims) {
      GkPrimInst *primInst;
      GkMaterial *material;

      primInst = rb_find(modelInst->prims, primi);
      material = modelMaterial;
      if (primInst)
        material = primInst->material;

      /* avoid uniform cached material if possible */
      if (prog->lastMaterial != material
          || prog->updtMaterials)
        gkUniformMaterial(prog, material);
    }

    if (primi->flags & GK_DRAW_ELEMENTS)
      glDrawElements(primi->mode,
                     primi->count,
                     GL_UNSIGNED_INT, /* TODO: ? */
                     NULL);
    else if (primi->flags & GK_DRAW_ARRAYS)
      glDrawArrays(primi->mode, 0, primi->count);

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
