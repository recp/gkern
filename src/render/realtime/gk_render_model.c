/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

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
  GkModelBase *modelBase;
  GkMatrix    *mat;
  GkProgInfo  *prog;
  uint32_t     updt;

  modelBase = modelInst->model;
  mat       = modelInst->matrix;
  prog      = modelBase->pinfo;

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
    modelBase->pinfo = prog = pprog;

  gkUniformMatrix(modelInst);

  /* pre events */
  if (modelBase->events && modelBase->events->onDraw)
    modelBase->events->onDraw(modelBase, NULL, false);

  /* render */
  if (modelBase->flags & GK_COMPLEX) {
    GkComplexModel *model;
    uint32_t        index;

    model = (GkComplexModel *)modelBase;

    for (index = 0; index < model->vaoCount; index++) {
      glBindVertexArray(model->vao[index]);

      if (modelBase->flags & GK_DRAW_ELEMENTS) {
        glDrawElements(model->modes[index],
                       model->count[index],
                       GL_UNSIGNED_INT, /* TODO: ? */
                       NULL);;
      } else if (modelBase->flags & GK_DRAW_ARRAYS) {
        glDrawArrays(model->modes[index],
                     0,
                     model->count[index]);
      }
    }
  } else {
    GkModel *model;
    model = (GkModel *)modelBase;

    glBindVertexArray(model->vao);

    if (modelBase->flags & GK_DRAW_ELEMENTS)
      glDrawElements(model->mode,
                     model->count,
                     GL_UNSIGNED_INT, /* TODO: ? */
                     NULL);
    else if (modelBase->flags & GK_DRAW_ARRAYS)
      glDrawArrays(model->mode, 0, model->count);
  }

  glBindVertexArray(0);

  /* post events */
  if (modelBase->events && modelBase->events->onDraw)
    modelBase->events->onDraw(modelBase, NULL, true);

  if(updt && mat != pmat)
    mat->cmatIsValid = 1;
}
