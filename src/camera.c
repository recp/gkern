/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "types/impl_camera.h"
#include "../include/gk/gk.h"
#include "../include/gk/camera.h"

#include <ds/forward-list.h>
#include <stdlib.h>
#include <string.h>

static
void
gkPrepareCameraProp(GkCamera * __restrict cam);

GK_EXPORT
GkCamera*
gkAllocCamera() {
  GkCameraImpl *camImpl;
  camImpl             = calloc(1, sizeof(*camImpl));
  camImpl->transfSlot = (1 << 30);
  return &camImpl->pub;
}

void
gkSetCamera(struct GkScene * __restrict scene,
            GkCamera       * __restrict camera) {
  scene->camera = camera;
  scene->camera->flags = 0;
}

GkCamera*
gkMakeCameraForScene(GkScene *scene) {
  vec3  target;
  vec3  eye;
  mat4  view, proj;
  float aspectRatio;

  aspectRatio = scene->vrect.size.w / scene->vrect.size.h;
  glm_vec_center(scene->bboxMin, scene->bboxMax, target);

  glm_vec_sub(scene->bboxMax, target, eye);
  glm_vec_scale(eye, 2.5f, eye); // TODO: read this as option
  glm_vec_add(target, eye, eye);

  glm_perspective_default(aspectRatio, proj);
  glm_lookat(eye,
             target,
             (vec3){0.0f, 1.0f, 0.0f}, // TODO: read this as option
             view);

  return gkMakeCamera(proj, view);
}

GkCamera*
gkMakeCamera(mat4 proj, mat4 view) {
  GkCamera *cam;

  cam = gkAllocCamera();

  glm_mat4_copy(proj, cam->proj);
  glm_mat4_copy(view, cam->view);
  glm_mat4_mul(proj, view, cam->viewProj);
  glm_mat4_inv_precise(view, cam->world);
  cam->trans = NULL;

  cam->flags = 0;

  return cam;
}

GkCamera*
gkMakeCameraByWorld(mat4 proj, mat4 view) {
  GkCamera *cam;

  cam = gkAllocCamera();

  glm_mat4_copy(proj, cam->proj);
  glm_mat4_copy(view, cam->world);
  glm_mat4_mul(proj, view, cam->viewProj);
  glm_mat4_inv_precise(view, cam->view);

  cam->trans = NULL;
  cam->flags = 0;

  return cam;
}

void
gkUpdateCamera(GkCamera *cam, mat4 proj, mat4 view) {
  glm_mat4_copy(proj, cam->proj);
  glm_mat4_copy(view, cam->view);
  glm_mat4_mul(proj, view, cam->viewProj);
  glm_mat4_inv_precise(view, cam->world);
  cam->trans = NULL;

  cam->flags = 0;
}

void
gkResizeCamera(GkCamera * __restrict camera,
               float aspectRatio) {
  glm_perspective_resize(aspectRatio, camera->proj);

  glm_mat4_mul(camera->proj,
               camera->view,
               camera->viewProj);
}

static
void
gkPrepareCameraProp(GkCamera * __restrict cam) {
  mat4 invViewProj;

  glm_mat4_inv(cam->viewProj, invViewProj);

  glm_frustum_planes(cam->viewProj, cam->frustum.planes);
  glm_frustum_corners(invViewProj, cam->frustum.corners);
  glm_frustum_center(cam->frustum.corners, cam->frustum.center);
}

void
gkCameraProjUpdated(GkCamera * __restrict cam) {
  glm_mat4_mul(cam->proj, cam->view, cam->viewProj);
  gkPrepareCameraProp(cam);
}

void
gkCameraViewUpdated(GkCamera * __restrict cam) {
  glm_mat4_inv(cam->world, cam->view);
  glm_mat4_mul(cam->proj,  cam->view, cam->viewProj);
  gkPrepareCameraProp(cam);
}

void
gkZoom(GkScene * __restrict scene,
       float distance) {
  GkCamera *cam;
  vec3      dir;

  if (!(cam = scene->camera) || distance == 0.0f)
    return;

  glm_vec_normalize_to(cam->world[2], dir);
  glm_vec_scale(dir, -distance, dir);
  glm_vec_add(cam->world[3], dir, cam->world[3]);

  gkCameraViewUpdated(cam);
  gkApplyView(scene, scene->rootNode);

  scene->flags |= GK_SCENEF_RENDER;
}

void
gkZoomInOneUnit(GkScene * __restrict scene) {
  gkZoom(scene, 1.0f);
}

void
gkZoomOutOneUnit(GkScene * __restrict scene) {
  gkZoom(scene, -1.0f);
}
