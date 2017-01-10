/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

GkModelInst *
gkMakeInstance(GkModelBase *model, GkMatrix *matrix) {
  GkModelInst *instance, *prevInstance;

  instance         = calloc(sizeof(*instance), 1);
  instance->matrix = matrix;
  prevInstance     = NULL;

  if (!model->instances)
    model->instances = calloc(sizeof(*model->instances), 1);
  else
    prevInstance = model->instances->instance;

  model->instances->instance = instance;
  model->instances->instanceCount++;

  if (prevInstance)
    instance->next = prevInstance;

  return instance;
}

void
gkUniformMatrix(GkModelBase *modelBase) {
  GkMatrix   *mat;
  GkProgInfo *pinfo;

  mat   = modelBase->matrix;
  pinfo = modelBase->pinfo;

  gkUniformMat4(pinfo->mvpi, *mat->cmvp);

  /* TODO: check GK_NON_UNIFORM_SCALE */
  if (modelBase->cnmat) {
    gkUniformMat4(pinfo->nmi, *modelBase->cnmat);
    glUniform1i(pinfo->unmi, 1);
  }
}
