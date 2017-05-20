/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_transform_h
#define gk_transform_h

#include "gk-common.h"
#include "gk-geom-types.h"
#include <cglm.h>

typedef enum GkMatrixFlags {
  GK_MATRIXF_NONE         = 0,
  GK_MATRIXF_CMAT         = 1 << 1,
  GK_MATRIXF_CMAT_ISVALID = (1 << 2) | GK_MATRIXF_CMAT,
  GK_MATRIXF_FMAT         = 1 << 3,
  GK_MATRIXF_FMAT_MV      = 1 << 4,
  GK_MATRIXF_FMAT_MVP     = 1 << 5,
  GK_MATRIXF_FMAT_NORMAT  = 1 << 6  /* use normal matrix or not */
} GkMatrixFlags;

typedef struct GkFinalMatrix {
  uint32_t refc;
  mat4     cmvp;  /* model view projection matrix */
  mat4     cmv;   /* model view matrix */
  mat4     cnmat; /* cached normal matrix */
} GkFinalMatrix;

/* some geometries or nodes may not have matrix,
 so they will use parent's one. */
typedef struct GkMatrix {
  uint32_t       refc;
  GkMatrixFlags  flags;

  /*
   cached MVP and normal matrix: proj * view * model (GkMatrix->cmat)
   because multiple models may share same mvp matrix, to save space we use
   pointer space in every matrix, GkMatrix is allocating by malloc so to meet
   aligment requirements this space will be wasted anyway.

   dont change order because mat4 requires 16 byte aligment!
   */
  GkFinalMatrix *fmat;
  mat4           matrix;
  mat4           cmat;  /* cached matrix multiplied with parent */
} GkMatrix;

GkPoint
gk_project2d(GkRect rect, mat4 mvp, vec3 v);

#endif /* gk_transform_h */
