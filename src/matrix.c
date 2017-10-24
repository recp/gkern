/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "matrix.h"

void
gkCalcViewMat(GkScene     * __restrict scene,
              GkTransform * __restrict tr) {
  GkFinalTransform *ftr;
  ftr = tr->ftr;
  if (!tr->ftr) {
    ftr       = malloc(sizeof(*tr->ftr));
    ftr->refc = 1;
    tr->ftr  = ftr;
  }

  glm_mat4_mul(scene->camera->view,
               tr->world,
               ftr->mv);

  tr->flags |= GK_TRANSF_FMAT | GK_TRANSF_FMAT_MV;
}

void
gkCalcFinalMat(GkScene     * __restrict scene,
               GkTransform * __restrict tr) {
  GkFinalTransform *ftr;

  ftr = tr->ftr;
  if (!tr->ftr) {
    ftr       = malloc(sizeof(*tr->ftr));
    ftr->refc = 1;
    tr->ftr   = ftr;
  }

  glm_mat4_mul(scene->camera->view,
               tr->world,
               ftr->mv);

  glm_mat4_mul(scene->camera->proj,
               ftr->mv,
               ftr->mvp);

  if (glm_uniscaled(tr->world)) {
    tr->flags &= ~GK_TRANSF_FMAT_NORMAT;
  } else {
    tr->flags |= GK_TRANSF_FMAT_NORMAT;

    glm_mat4_inv(ftr->mv, ftr->nm);
    glm_mat4_transpose(ftr->nm);
  }

  tr->flags |= GK_TRANSF_FMAT
               | GK_TRANSF_FMAT_MV
               | GK_TRANSF_FMAT_MVP;
}
