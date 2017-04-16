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

typedef struct GkFinalMatrix {
  uint32_t refc;
  uint32_t usenm; /* use generated normal matrix when non-uniform scaled */
  mat4     cmvp;  /* model view projection matrix */
  mat4     cmv;   /* model view matrix */
  mat4     cnmat; /* cached normal matrix */
} GkFinalMatrix;

/* some geometries or nodes may not have matrix,
 so they will use parent's one. */
typedef struct GkMatrix {
  uint32_t       refc;
  uint32_t       cmatIsValid;
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
