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

#ifndef gk_camera_h
#define gk_camera_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "transform.h"
#include "plane.h"
#include "bbox.h"
#include <cglm/cglm.h>

struct GkScene;
struct GkGeometryInst;
struct GkMaterial;
struct GkPrimitive;
struct GkPrimInst;
struct FListItem;

typedef enum GkCameraFlags {
  GK_CAMF_NONE     = 0,
  GK_UPDT_VIEW     = 1 << 0,
  GK_UPDT_PROJ     = 1 << 2,
  GK_UPDT_VIEWPROJ = GK_UPDT_VIEW | GK_UPDT_PROJ
} GkCameraFlags;

GK_MAKE_C_ENUM(GkCameraFlags)

typedef struct GkRenderList {
  size_t             size;
  size_t             count;
  struct GkPrimInst *items[];
} GkRenderList;

typedef struct GkFrustum {
  vec4              center;
  GkPlane           planes[6]; /* left, right, bottom, top, near, far */
  vec4              corners[8];
    
  GkRenderList     *opaque;
  GkRenderList     *transp;
  struct FListItem *modelInsList;
} GkFrustum;

typedef struct GkCamera {
  mat4           proj;
  mat4           view;
  mat4           world;
  mat4           viewProj;
  GkFrustum      frustum;
  GkCameraFlags  flags;
} GkCamera;

GK_EXPORT
GkCamera*
gkAllocCamera(void);

GkCamera*
gkMakeCameraForScene(struct GkScene *scene);

GkCamera*
gkMakeCamera(mat4 proj, mat4 view);

GkCamera*
gkMakeCameraByWorld(mat4 proj, mat4 view);

void
gkUpdateCamera(GkCamera *cam, mat4 proj, mat4 view);

void
gkSetCamera(struct GkScene * __restrict scene,
            GkCamera       * __restrict camera);

void
gkResizeCamera(GkCamera * __restrict camera,
               float aspectRatio);

void
gkCameraProjUpdated(GkCamera * __restrict cam);

void
gkCameraViewUpdated(GkCamera * __restrict cam);

void
gkCameraWorldUpdated(GkCamera * __restrict cam);

void
gkZoom(struct GkScene * __restrict scene,
       float distance);

void
gkZoomInOneUnit(struct GkScene * __restrict scene);

void
gkZoomOutOneUnit(struct GkScene * __restrict scene);

GK_EXPORT
void
gkCullFrustum(struct GkScene * __restrict scene,
              GkCamera       * __restrict cam);

GK_EXPORT
void
gkCullSubFrustum(GkFrustum * __restrict frustum,
                 GkFrustum * __restrict subfrustum);

GK_EXPORT
void
gkBoxInFrustum(GkFrustum * __restrict frustum,
               vec3                   box[2]);

#ifdef __cplusplus
}
#endif
#endif /* gk_camera_h */
