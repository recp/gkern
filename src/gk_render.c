/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

static
inline
void
gkRenderSimple(GkModelInstance * modelInstance) {
  GkModel *model;
  model = (GkModel *)modelInstance->model;

  glBindVertexArray(model->vao);

  if (modelInstance->flags & GK_DRAW_ELEMENTS)
    glDrawElements(model->mode, 0, model->mode, NULL);
  else if (modelInstance->flags & GK_DRAW_ARRAYS)
    glDrawArrays(model->mode, 0, model->count);

  glBindVertexArray(0);
}

static
inline
void
gkRenderComplex(GkModelInstance * modelInstance) {
  GkComplexModel * __restrict model;
  uint32_t index;

  model = (GkComplexModel *)modelInstance->model;

  for (index = 0; index < model->vaoCount; index++) {
    glBindVertexArray(model->vao[index]);

    if (model->base.flags & GK_DRAW_ELEMENTS) {
      glDrawElements(model->modes[index],
                     model->count[index],
                     GL_UNSIGNED_INT, /* TODO: ? */
                     NULL);;
    } else if (modelInstance->flags & GK_DRAW_ARRAYS) {
      glDrawArrays(model->modes[index],
                   0,
                   model->count[index]);
    }

    glBindVertexArray(0);
  }
}

/*---------------------------------------------------------------------------*/

void
gkRender(GkModelInstance * modelInstance) {
  GkModelBase *model;

  model = modelInstance->model;

  /* pre events */
  if (modelInstance->onDraw)
    modelInstance->onDraw(model, modelInstance, false);

  if (model->onDraw)
    model->onDraw(model, modelInstance, false);

  gkUniformModelMatrix(modelInstance);

  /* render */
  if (model->flags & GK_COMPLEX)
    gkRenderComplex(modelInstance);
  else
    gkRenderSimple(modelInstance);

  /* post events */
  if (modelInstance->onDraw)
    modelInstance->onDraw(model, modelInstance, true);

  if (model->onDraw)
    model->onDraw(model, modelInstance, true);
}
