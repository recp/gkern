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

#ifndef gk_scene_h
#define gk_scene_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "renderpath.h"

struct GkOutput;
struct FListItem;
struct GkScene;
struct GkLight;
struct GkContext;
struct GkMaterial;
struct GkModelInst;
struct GkPrimInst;
struct GkNode;
struct GkPipeline;
struct GkLightRef;

typedef void (*GkRenderPrimFunc)(struct GkScene    *scene,
                                 struct GkPrimInst *primInst);

typedef enum GkSceneFlags {
  GK_SCENEF_NONE           = 0,
  GK_SCENEF_DRAW_BBOX      = 1 << 0,
  GK_SCENEF_UPDT_LIGHTS    = 1 << 3,
  GK_SCENEF_ONCE           = 1 << 4,
  GK_SCENEF_RENDERING      = 1 << 5,
  GK_SCENEF_RENDERED       = 1 << 6,
  GK_SCENEF_RENDER         = 1 << 7,
  GK_SCENEF_NEEDS_RENDER   = GK_SCENEF_RENDER | GK_SCENEF_UPDT_LIGHTS,
  GK_SCENEF_INIT           = GK_SCENEF_NEEDS_RENDER,
  GK_SCENEF_TRANSP         = 1 << 8,
  GK_SCENEF_SHADOWS        = 1 << 9,
  GK_SCENEF_PREPARED       = 1 << 10,
  GK_SCENEF_DRAW_PRIM_BBOX = 1 << 11,
  GK_SCENEF_DRAW_BONES     = 1 << 12
} GkSceneFlags;

GK_MAKE_C_ENUM(GkSceneFlags)

typedef struct GkScene {
  struct GkContext  *ctx;
  GkCamera          *camera;
  GkCamera          *subCamera;
  GkTransform       *trans;  /* free camera */
  struct GkNode     *rootNode;
  struct GkLightRef *lights;
  struct GkOutput   *finalOutput; /* set NULL for default FBO (screen) */
  GkBBox             bbox;
  GkRenderPrimFunc   renderPrimFunc;
  vec4               viewport;
  uint32_t           lightCount;
  uint32_t           lastLightIndex;
  GkSceneFlags       flags;
  int32_t            internalFormat;
  float              backingScale;
  float              fpsApprx;
  vec3               center;
  double             startTime;
  double             endTime;
} GkScene;

GK_INLINE
GkCamera*
gkActiveCamera(GkScene * __restrict scene) {
  if (!scene->subCamera)
    return scene->camera;
  return scene->subCamera;
}

GK_EXPORT
GkScene*
gkAllocScene(struct GkContext * __restrict context);

GK_EXPORT
void
gkPrepareScene(GkScene * __restrict scene);

GK_INLINE
struct GkContext*
gkContextOf(GkScene * __restrict scene) {
  return scene->ctx;
}

GK_EXPORT
void
gkEnableShadows(GkScene * __restrict scene);

GK_EXPORT
void
gkDisableShadows(GkScene * __restrict scene);

GK_EXPORT
void
gkCacheTransformsFor(GkScene  * __restrict scene,
                     GkCamera * __restrict cam);

GK_EXPORT
void
gkRemoveTransformCacheFor(GkScene  * __restrict scene,
                          GkCamera * __restrict cam);

GK_EXPORT
bool
gkSceneIs2D(GkScene *scene);

#ifdef __cplusplus
}
#endif
#endif /* gk_scene_h */
