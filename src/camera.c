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
#include <stdlib.h>
#include <string.h>

GK_EXPORT
GkCamera*
gkAllocCamera() {
  GkCameraImpl *camImpl;
  camImpl             = calloc(sizeof(*camImpl), 1);
  camImpl->transfSlot = (1 << 31);
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
  glm_vec_copy(scene->bbox->center, target);

  glm_vec_sub(scene->bbox->max, target, eye);
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
  glm_mat4_mul(proj, view, cam->projView);
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
  glm_mat4_mul(proj, view, cam->projView);
  glm_mat4_inv_precise(view, cam->view);
  cam->trans = NULL;

  cam->flags = 0;

  return cam;
}

void
gkResizeCamera(GkCamera * __restrict camera,
               float aspectRatio) {
  glm_perspective_resize(aspectRatio, camera->proj);

  glm_mat4_mul(camera->proj,
               camera->view,
               camera->projView);
}

void
gkUpdateCameraView(GkCamera * __restrict cam) {
  glm_mat4_inv(cam->world, cam->view);
  glm_mat4_mul(cam->proj,
               cam->view,
               cam->projView);
}

void
gkUpdateCameraWorld(GkCamera * __restrict cam) {
  glm_mat4_inv(cam->world, cam->view);
  glm_mat4_mul(cam->proj,
               cam->view,
               cam->projView);
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

  gkUpdateCameraView(cam);

  scene->camera->flags |= GK_UPDT_VIEW;
  scene->flags         |= GK_SCENEF_RENDER;
}

void
gkZoomInOneUnit(GkScene * __restrict scene) {
  gkZoom(scene, 1.0f);
}

void
gkZoomOutOneUnit(GkScene * __restrict scene) {
  gkZoom(scene, -1.0f);
}
