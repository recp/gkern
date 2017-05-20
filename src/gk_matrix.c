/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_matrix.h"

void
gkCalcViewMat(GkScene  * __restrict scene,
              GkMatrix * __restrict mat) {
  GkFinalMatrix *fmat;
  fmat = mat->fmat;
  if (!mat->fmat) {
    fmat       = malloc(sizeof(*mat->fmat));
    fmat->refc = 1;
    mat->fmat  = fmat;
  }

  glm_mat4_mul(scene->camera->view,
               mat->cmat,
               fmat->cmv);

  mat->flags |= GK_MATRIXF_FMAT | GK_MATRIXF_FMAT_MV;
}

void
gkCalcFinalMat(GkScene  * __restrict scene,
               GkMatrix * __restrict mat) {
  GkFinalMatrix *fmat;

  fmat = mat->fmat;
  if (!mat->fmat) {
    fmat       = malloc(sizeof(*mat->fmat));
    fmat->refc = 1;
    mat->fmat  = fmat;
  }

  glm_mat4_mul(scene->camera->view,
               mat->cmat,
               fmat->cmv);

  glm_mat4_mul(scene->camera->proj,
               fmat->cmv,
               fmat->cmvp);

  if (glm_uniscaled(mat->cmat)) {
    mat->flags &= ~GK_MATRIXF_FMAT_NORMAT;
  } else {
    mat->flags |= GK_MATRIXF_FMAT_NORMAT;

    glm_mat4_inv(fmat->cmv, fmat->cnmat);
    glm_mat4_transpose(fmat->cnmat);
  }

  mat->flags |= GK_MATRIXF_FMAT
                | GK_MATRIXF_FMAT_MV
                | GK_MATRIXF_FMAT_MVP;
}
