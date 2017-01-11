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
  GkFinalMatrix *fmat;
  GkProgInfo    *pinfo;

  fmat  = modelBase->matrix->fmat;
  pinfo = modelBase->pinfo;

  /* Model View Projection Matrix */
  gkUniformMat4(pinfo->mvpi, fmat->cmvp);

  /* Model View Matrix */
  gkUniformMat4(pinfo->mvi, fmat->cmv);

  /* Normal Matrix */
  if (fmat->usenm)
    gkUniformMat4(pinfo->nmi,  fmat->cnmat);

  glUniform1i(pinfo->nmui, fmat->usenm);
}
