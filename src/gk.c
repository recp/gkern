/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

GkModelInst *
gkMakeInstance(GkModel *model) {
  GkModelInst *instance, *prevInstance;

  instance     = calloc(sizeof(*instance), 1);
  prevInstance = NULL;

  if (!model->instances)
    model->instances = calloc(sizeof(*model->instances), 1);
  else
    prevInstance = model->instances->instance;

  model->instances->instance = instance;
  model->instances->instanceCount++;
  instance->model = model;
  
  if (prevInstance)
    instance->next = prevInstance;

  return instance;
}

void
gkUniformMatrix(GkModelInst *modelInst) {
  GkModel       *model;
  GkFinalMatrix *fmat;
  GkProgInfo    *pinfo;

  model = modelInst->model;

  fmat  = modelInst->matrix->fmat;
  pinfo = model->pinfo;

  /* Model View Projection Matrix */
  gkUniformMat4(pinfo->mvpi, fmat->cmvp);

  /* Model View Matrix */
  gkUniformMat4(pinfo->mvi, fmat->cmv);

  /* Normal Matrix */
  if (fmat->usenm)
    gkUniformMat4(pinfo->nmi,  fmat->cnmat);

  glUniform1i(pinfo->nmui, fmat->usenm);
}
