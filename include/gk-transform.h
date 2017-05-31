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
  GK_TRANSF_NONE          =  0,
  GK_TRANSF_LOCAL         = (1 << 0),
  GK_TRANSF_LOCAL_ISVALID = (1 << 1) | GK_TRANSF_LOCAL,
  GK_TRANSF_WORLD         = (1 << 2),
  GK_TRANSF_WORLD_ISVALID = (1 << 3) | GK_TRANSF_WORLD,
  GK_TRANSF_FMAT          =  1 << 4,
  GK_TRANSF_FMAT_MV       =  1 << 5,
  GK_TRANSF_FMAT_MVP      =  1 << 6,
  GK_TRANSF_FMAT_NORMAT   =  1 << 7  /* use normal matrix or not */
} GkTransformFlags;

typedef enum GkTransformType {
  GK_TRANS_LOOK_AT   = 1,
  GK_TRANS_MATRIX    = 2,
  GK_TRANS_ROTATE    = 3,
  GK_TRANS_SCALE     = 4,
  GK_TRANS_SKEW      = 5,
  GK_TRANS_TRANSLATE = 6
} GkTransformType;

typedef struct GkTransformItem {
  struct GkTransformItem *prev;
  struct GkTransformItem *next;
  GkTransformType         type;
} GkTransformItem;

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
  GkTransformItem  *item;   /* individual transforms               */
  GkFinalTransform *ftr;    /* shader ready transform[s] and infos */
  mat4              local;  /* cached local transform as matrix    */
  mat4              world;  /* cached world transform as matrix    */
} GkTransform;

/* individual transforms */

typedef struct GkMatrix {
  GkTransformItem base;
  mat4            value;
} GkMatrix;

typedef struct GkLookAt {
  GkTransformItem base;
  vec3            value[3];
} GkLookAt;

typedef struct GkRotate {
  GkTransformItem base;
  vec4            value;
} GkRotate;

typedef struct GkScale {
  GkTransformItem base;
  vec3            value;
} GkScale;

typedef struct GkTranslate {
  GkTransformItem base;
  vec3            value;
} GkTranslate;

typedef struct GkSkew {
  GkTransformItem base;
  float           angle;
  vec3            rotateAxis;
  vec3            aroundAxis;
} GkSkew;

void
gkTransformCombine(GkTransform * __restrict trans);

GkPoint
gk_project2d(GkRect rect, mat4 mvp, vec3 v);

#endif /* gk_transform_h */
