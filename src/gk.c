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
gkUniformModelMatrix(GkModelBase *modelBase) {
  gkUniformMat4(modelBase->cmat.index,
                modelBase->cmat.matrix);
}

void
gkUniformInstanceMatrix(GkModelInst *instance) {
  gkUniformMat4(instance->matrix->index,
                instance->matrix->matrix);
}
