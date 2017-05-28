/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_camera_h
#define gk_camera_h

#include "gk-common.h"
#include <cglm.h>

struct GkScene;

typedef enum GkCameraFlags {
  GK_CAMERA_FLAGS_NONE = 0,
  GK_PROJVIEW_IS_VALID = 1 << 0,
  GK_VIEW_IS_VALID     = 2 << 0
} GkCameraFlags;

typedef struct GkCamera {
  mat4          proj;
  mat4          view;
  mat4          world;
  mat4          projView;
  vec3          dir;
  GkCameraFlags flags;
  GkTransform   *trans;
} GkCamera;

GkCamera*
gkMakeCameraForScene(struct GkScene *scene);

GkCamera*
gkMakeCamera(mat4 proj, mat4 view);

GkCamera*
gkMakeCameraByWorld(mat4 proj, mat4 view);

void
gkSetCamera(struct GkScene * __restrict scene,
            GkCamera       * __restrict camera);

void
gkResizeCamera(GkCamera * __restrict camera,
               float aspectRatio);

void
gkUpdateCameraView(GkCamera * __restrict cam);

void
gkUpdateCameraWorld(GkCamera * __restrict cam);

void
gkZoom(struct GkScene * __restrict scene,
       float distance);

void
gkZoomInOneUnit(struct GkScene * __restrict scene);

void
gkZoomOutOneUnit(struct GkScene * __restrict scene);

#endif /* gk_camera_h */
