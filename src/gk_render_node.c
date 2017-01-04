/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

void
gkRenderNode(GkNode   *node,
             GkMatrix *parentTrans) {
  while (node) {
    GkMatrix *cmat;
    if (parentTrans || !node->cachedMatrixIsValid) {
      cmat = &node->cachedMatrix;

      if (node->matrix) {
        if (node->matrix->index == -1)
          cmat->index = parentTrans->index;
        else
          cmat->index = node->matrix->index;

        glm_mat4_mul(parentTrans->matrix,
                     node->matrix->matrix,
                     cmat->matrix);
      } else {
        glm_mat4_dup(parentTrans->matrix,
                     cmat->matrix);
        cmat->index = parentTrans->index;
      }

      node->cachedMatrixIsValid = 1;
    } else {
      cmat = NULL;
    }

    if (node->model) {
      gkRenderModel(node->model, cmat);
    } else if (node->instance) {
      gkRenderInstance(node->instance, cmat);
    }

    if (node->chld)
      gkRenderNode(node->chld, cmat);

    if (node->nodeInst)
      gkRenderNode(node->nodeInst, cmat);

    node = node->next;
  }
}
