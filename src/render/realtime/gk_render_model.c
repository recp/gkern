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
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst,
              GkMatrix    *pmat,
              GkProgInfo  *pprog) {
  GkModel     *model;
  GkPrimitive *primi;
  GkMatrix    *mat;
  GkProgInfo  *prog;
  GkMaterial  *modelMaterial;
  uint32_t     updt;

  model = modelInst->model;
  mat   = modelInst->matrix;
  prog  = model->pinfo;

  if (!mat)
    modelInst->matrix = mat = pmat;

  updt = !pmat->cmatIsValid || !mat->cmatIsValid;

  if (updt){
    if (pmat != mat) {
      glm_mat4_mul(pmat->matrix,
                   mat->matrix,
                   mat->cmat);
      mat->cmatIsValid = 0;
    }

    gkCalcFinalMat(scene, mat);
  } else if(scene->flags & (GK_SCENEF_UPDT_VIEW | GK_SCENEF_UPDT_VIEWPROJ)) {
    gkCalcFinalMat(scene, mat);
  }

  if (!prog)
    model->pinfo = prog = pprog;

  gkUniformMatrix(modelInst);

  /* model's material */
  modelMaterial = NULL;
  if (!modelInst->prims
      || modelInst->material
      || modelInst->model->material) {
    modelMaterial = modelInst->material;
    if (!modelMaterial)
      modelMaterial = modelInst->model->material;

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

  if(updt && mat != pmat)
    mat->cmatIsValid = 1;

  if ((model->flags & GK_MODEL_FLAGS_DRAW_BBOX)
      && model->bbox)
    gkDrawBBox(scene,
               mat->cmat,
               model->bbox->min,
               model->bbox->max);
}
