/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

void
gkRenderScene(GkScene * scene) {
  GkMatrix *cmat;
  cmat = scene->mat;
  if (!cmat) {
    cmat        = malloc(sizeof(*cmat->matrix));
    cmat->index = scene->matrixLoc;
    glm_mat4_dup(GLM_MAT4_IDENTITY, cmat->matrix);

    scene->mat = cmat;
    gkRenderNode(scene->rootNode, cmat);
  } else {
    gkRenderNode(scene->rootNode, NULL);
  }
}
