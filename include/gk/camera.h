/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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

typedef enum GkCameraFlags {
  GK_CAMF_NONE     = 0,
  GK_UPDT_VIEW     = 1 << 0,
  GK_UPDT_PROJ     = 1 << 2,
  GK_UPDT_VIEWPROJ = GK_UPDT_VIEW | GK_UPDT_PROJ
} GkCameraFlags;

GK_MAKE_C_ENUM(GkCameraFlags)

typedef struct GkCamera {
  mat4           proj;
  mat4           view;
  mat4           world;
  mat4           projView;
  GkPlane        planes[6]; /* left, right, bottom, top, near, far */
  vec4           vertices[8];
  GkBBox         bbox;
  GkCameraFlags  flags;
  GkTransform   *trans;
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
gkZoom(struct GkScene * __restrict scene,
       float distance);

void
gkZoomInOneUnit(struct GkScene * __restrict scene);

void
gkZoomOutOneUnit(struct GkScene * __restrict scene);

GK_EXPORT
void
gkExtractPlanes(mat4 projView, GkPlane planes[6]);

#ifdef __cplusplus
}
#endif
#endif /* gk_camera_h */
