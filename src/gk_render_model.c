/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

void
gkRenderModel(GkModelBase * modelBase) {
  /* pre events */
  if (modelBase->events && modelBase->events->onDraw)
    modelBase->events->onDraw(modelBase, NULL, false);

  if (modelBase->matrix)
    gkUniformModelMatrix(modelBase);

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
