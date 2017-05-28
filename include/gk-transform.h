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

typedef enum GkTransformFlags {
  GK_TRANSF_NONE         = 0,
  GK_TRANSF_WORLD         = 1 << 1,
  GK_TRANSF_WORLD_ISVALID = (1 << 2) | GK_TRANSF_WORLD,
  GK_TRANSF_FMAT         = 1 << 3,
  GK_TRANSF_FMAT_MV      = 1 << 4,
  GK_TRANSF_FMAT_MVP     = 1 << 5,
  GK_TRANSF_FMAT_NORMAT  = 1 << 6  /* use normal matrix or not */
} GkTransformFlags;

typedef struct GkFinalTransform {
  uint32_t refc;
  mat4     mvp;  /* model view projection matrix */
  mat4     mv;   /* model view matrix            */
  mat4     nm;   /* normal matrix                */
} GkFinalTransform;

/* some geometries or nodes may not have matrix,
   so they will use parent's one. */
typedef struct GkTransform {
  uint32_t          refc;
  GkTransformFlags  flags;
  GkFinalTransform *ftr;    /* shader ready transform[s] and infos */
  mat4              local;  /* cached local transform as matrix    */
  mat4              world;  /* cached world transform as matrix    */
} GkTransform;

GkPoint
gk_project2d(GkRect rect, mat4 mvp, vec3 v);

#endif /* gk_transform_h */
