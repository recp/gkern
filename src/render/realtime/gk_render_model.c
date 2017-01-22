/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk.h"
#include "../../../include/gk-material.h"

GK_INLINE
void
gkCalcFinalMat(GkScene  * __restrict scene,
               GkMatrix * __restrict mat) {
  GkFinalMatrix *fmat;
  fmat = mat->fmat;
  if (!mat->fmat) {
    fmat       = malloc(sizeof(*mat->fmat));
    fmat->refc = 1;
    mat->fmat  = fmat;
  }

  glm_mat4_mul(scene->pv, mat->cmat, fmat->cmvp);
  glm_mat4_mul(scene->v,  mat->cmat, fmat->cmv);

  fmat->usenm = !glm_uniscaled(mat->cmat);

  if (fmat->usenm) {
    glm_mat4_inv(fmat->cmv, fmat->cnmat);
    glm_mat4_transpose(fmat->cnmat);
  }
}

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst,
              GkMatrix    *pmat,
              GkProgInfo  *pprog) {
  GkModel     *model;
  GkPrimitive *primi;
  GkMatrix    *mat;
  GkProgInfo  *prog;
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
  } else if(!scene->pvIsValid || scene->vIsValid) {
    gkCalcFinalMat(scene, mat);
  }

  if (!prog)
    model->pinfo = prog = pprog;

  gkUniformMatrix(modelInst);
  gkUniformMaterial(scene, modelInst);

  /* pre events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, false);

  /* render */
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);

    if (primi->flags & GK_DRAW_ELEMENTS)
      glDrawElements(primi->mode,
                     primi->count,
                     GL_UNSIGNED_INT, /* TODO: ? */
                     NULL);
    else if (primi->flags & GK_DRAW_ARRAYS)
      glDrawArrays(primi->mode, 0, primi->count);

    primi = primi->next;
  }

  glBindVertexArray(0);

  /* post events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, true);

  if(updt && mat != pmat)
    mat->cmatIsValid = 1;
}
