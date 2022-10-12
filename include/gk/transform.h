/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef gk_transform_h
#define gk_transform_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "geom-types.h"
#include <cglm/cglm.h>

struct GkScene;
struct GkPipeline;
struct GkCamera;

typedef enum GkTransformFlags {
  GK_TRANSF_NONE          =  0,
  GK_TRANSF_LOCAL         = (1 << 0),
  GK_TRANSF_LOCAL_ISVALID = (1 << 1) | GK_TRANSF_LOCAL,
  GK_TRANSF_WORLD         = (1 << 2),
  GK_TRANSF_WORLD_ISVALID = (1 << 3) | GK_TRANSF_WORLD,
  GK_TRANSF_FMAT          =  1 << 4,
  GK_TRANSF_FMAT_MV       =  1 << 5,
  GK_TRANSF_FMAT_MVP      =  1 << 6,
  GK_TRANSF_FMAT_NORMAT   =  1 << 7,  /* use normal matrix or not */
  GK_TRANSF_CALC_VIEW     =  1 << 8
} GkTransformFlags;

typedef enum GkTransformType {
  GK_TRANS_LOOK_AT   = 1,
  GK_TRANS_MATRIX    = 2,
  GK_TRANS_ROTATE    = 3,
  GK_TRANS_SCALE     = 4,
  GK_TRANS_SKEW      = 5,
  GK_TRANS_TRANSLATE = 6,
  GK_TRANS_QUAT      = 7
} GkTransformType;

typedef struct GkTransformItem {
  struct GkTransformItem *prev;
  struct GkTransformItem *next;
  GkTransformType         type;
} GkTransformItem;

/* some geometries or nodes may not have matrix,
 so they will use parent's one. */
typedef struct GkTransform {
  mat4              local;  /* cached local transform as matrix         */
  mat4              world;  /* cached world transform as matrix         */
  GkTransformFlags  flags;
  GkTransformItem  *item;   /* individual transforms                    */
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

typedef struct GkQuaternion {
  GkTransformItem base;
  versor          value;
} GkQuaternion;

typedef struct GkSkew {
  GkTransformItem base;
  float           angle;
  vec3            rotateAxis;
  vec3            aroundAxis;
} GkSkew;

GK_EXPORT
GkTransform*
gkAllocTransform(struct GkScene * __restrict scene);

void
gkTransformCombine(GkTransform * __restrict trans);

GkPoint
gk_project2d(GkRect rect, mat4 mvp, vec3 v);

#ifdef __cplusplus
}
#endif
#endif /* gk_transform_h */
