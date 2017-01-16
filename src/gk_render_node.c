/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

void
gkRenderNode(GkScene    *scene,
             GkNode     *node,
             GkMatrix   *pmat,
             GkProgInfo *pprog) {
  while (node) {
    GkMatrix   *mat;
    GkProgInfo *prog;
    uint32_t    updt;

    mat  = node->matrix;
    prog = node->pinfo;

    if (!mat)
      node->matrix = mat = pmat;

    updt = (!pmat->cmatIsValid || !mat->cmatIsValid);

    if (updt && pmat != mat) {
      glm_mat4_mul(pmat->cmat,
                   mat->matrix,
                   mat->cmat);

      mat->cmatIsValid = 0;
    }

    if (!prog)
      node->pinfo = prog = pprog;

    if (node->model)
      gkRenderModel(scene,
                    node->model,
                    mat,
                    prog);

    if (node->chld)
      gkRenderNode(scene,
                   node->chld,
                   mat,
                   prog);

    if (node->nodeInst)
      gkRenderNode(scene,
                   node->nodeInst,
                   mat,
                   prog);

    if (updt && mat != pmat)
      mat->cmatIsValid = 1;

    node = node->next;
  }
}

void
gkPrepNode(GkScene    *scene,
           GkNode     *node,
           GkMatrix   *pmat,
           GkProgInfo *pprog) {
  while (node) {
    GkMatrix   *mat;
    GkProgInfo *prog;
    uint32_t    updt;

    mat  = node->matrix;
    prog = node->pinfo;

    if (!mat)
      node->matrix = mat = pmat;

    updt = (!pmat->cmatIsValid || !mat->cmatIsValid);

    if (updt && pmat != mat) {
      glm_mat4_mul(pmat->cmat, mat->matrix, mat->cmat);
      mat->cmatIsValid = 0;
    }

    if (!prog)
      node->pinfo = prog = pprog;

    if (node->chld)
      gkPrepNode(scene, node->chld, mat, prog);

    if (node->nodeInst)
      gkPrepNode(scene, node->nodeInst, mat, prog);

    if (updt && mat != pmat)
      mat->cmatIsValid = 1;
    
    node = node->next;
  }
}
