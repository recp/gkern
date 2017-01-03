/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

void
gkRenderModel(GkModelBase *modelBase,
              GkMatrix    *parentTrans) {
  if (parentTrans || !modelBase->cachedMatrixIsValid) {
    if (modelBase->matrix) {
      if (modelBase->matrix->index == -1)
        modelBase->cachedMatrix.index = parentTrans->index;
      else
        modelBase->cachedMatrix.index = modelBase->matrix->index;

      glm_mat4_mul(parentTrans->matrix,
                   modelBase->matrix->matrix,
                   modelBase->cachedMatrix.matrix);
    } else {
      modelBase->cachedMatrix.index = parentTrans->index;
      glm_mat4_dup(parentTrans->matrix,
                   modelBase->cachedMatrix.matrix);
      modelBase->cachedMatrix.index = parentTrans->index;
    }

    modelBase->cachedMatrixIsValid = 1;
    gkUniformModelMatrix(modelBase);
  }

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
}
