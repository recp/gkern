/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk.h"
#include "../../gk_matrix.h"

void
gkRenderNode(GkScene    *scene,
             GkNode     *node,
             GkMatrix   *pmat,
             GkProgInfo *pprog) {
  while (node) {
    GkMatrix   *mat;
    GkProgInfo *prog;

    mat  = node->matrix;
    prog = node->pinfo;

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

    updt = !((pmat->flags & mat->flags) & GK_MATRIXF_CMAT_ISVALID);

    if (updt && pmat != mat) {
      glm_mat4_mul(pmat->cmat,
                   mat->matrix,
                   mat->cmat);
      mat->flags &= ~GK_MATRIXF_CMAT_ISVALID;
    }

    if (!prog)
      node->pinfo = prog = pprog;

    if (node->light) {
      if (!(mat->flags & GK_MATRIXF_CMAT_ISVALID)
          || !(mat->flags & GK_MATRIXF_FMAT_MV)
          || scene->flags & GK_SCENEF_UPDT_VIEW) {
        gkCalcViewMat(scene, mat);
        gkUniformLightPos(node);
      }
    }

    if (node->model)
      gkPrepModel(scene,
                  node->model,
                  mat,
                  prog);

    if (node->chld)
      gkPrepNode(scene, node->chld, mat, prog);

    if (node->nodeInst)
      gkPrepNode(scene, node->nodeInst, mat, prog);

    if (updt && mat != pmat)
      mat->flags |= GK_MATRIXF_CMAT_ISVALID;
    
    node = node->next;
  }
}
