/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

void
gkRenderInstance(GkScene     *scene,
                 GkModelInst *instance,
                 GkMatrix    *parentMat,
                 GkProgInfo  *parentProg) {
  GkModelBase *model;

  model = instance->model;

  /* pre events */
  if (instance->events && instance->events->onDraw)
    instance->events->onDraw(model, instance, false);

  if (model->events && model->events->onDraw)
    model->events->onDraw(model, instance, false);

  /* render */
  if (model->flags & GK_COMPLEX) {
    GkComplexModel * __restrict model;
    uint32_t index;

    model = (GkComplexModel *)instance->model;

    for (index = 0; index < model->vaoCount; index++) {
      glBindVertexArray(model->vao[index]);

      if (model->base.flags & GK_DRAW_ELEMENTS) {
        glDrawElements(model->modes[index],
                       model->count[index],
                       GL_UNSIGNED_INT, /* TODO: ? */
                       NULL);;
      } else if (instance->flags & GK_DRAW_ARRAYS) {
        glDrawArrays(model->modes[index],
                     0,
                     model->count[index]);
      }
    }
  } else {
    GkModel *model;
    model = (GkModel *)instance->model;

    glBindVertexArray(model->vao);

    if (instance->flags & GK_DRAW_ELEMENTS)
      glDrawElements(model->mode,
                     model->count,
                     GL_UNSIGNED_INT, /* TODO: ? */
                     NULL);
    else if (instance->flags & GK_DRAW_ARRAYS)
      glDrawArrays(model->mode, 0, model->count);
  }

  glBindVertexArray(0);
  
  /* post events */
  if (instance->events && instance->events->onDraw)
    instance->events->onDraw(model, instance, true);

  if (model->events && model->events->onDraw)
    model->events->onDraw(model, instance, true);
}
