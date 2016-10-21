/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

GkModelInstance *
gkMakeInstance(GkModelBase *model,
               GLint matrixLoc) {
  GkModelInstance *instance;

  instance            = calloc(sizeof(*instance), 1);
  instance->matrixLoc = matrixLoc;
  instance->model     = model;

  instance->matrix[0][0] = 1.0f;
  instance->matrix[1][1] = 1.0f;
  instance->matrix[2][2] = 1.0f;
  instance->matrix[3][3] = 1.0f;

  return instance;
}

void
gkUniformModelMatrix(GkModelBase *modelBase) {
  gkUniformMat4(modelBase->matrixLoc,
                modelBase->matrix);
}

void
gkUniformInstanceMatrix(GkModelInstance *modelInstance) {
  gkUniformMat4(modelInstance->matrixLoc,
                modelInstance->matrix);
}
