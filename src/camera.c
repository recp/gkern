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

static
void
gkPrepareCameraProp(GkCamera * __restrict cam) {
  mat4     projViewInv;
  vec4    *vert;
  vec3     min, max;
  int32_t  i;

  glm_extract_planes(cam->projView, cam->frustum.planes);

  vert = cam->vertices;
  glm_mat4_inv(cam->projView, projViewInv);

  glm_mat4_mulv(projViewInv, (vec4){-1.0f, -1.0f, -1.0f, 1.0f}, vert[0]);
  glm_mat4_mulv(projViewInv, (vec4){-1.0f,  1.0f, -1.0f, 1.0f}, vert[1]);
  glm_mat4_mulv(projViewInv, (vec4){ 1.0f, -1.0f, -1.0f, 1.0f}, vert[2]);
  glm_mat4_mulv(projViewInv, (vec4){ 1.0f,  1.0f, -1.0f, 1.0f}, vert[3]);

  glm_mat4_mulv(projViewInv, (vec4){-1.0f, -1.0f,  1.0f, 1.0f}, vert[4]);
  glm_mat4_mulv(projViewInv, (vec4){-1.0f,  1.0f,  1.0f, 1.0f}, vert[5]);
  glm_mat4_mulv(projViewInv, (vec4){ 1.0f, -1.0f,  1.0f, 1.0f}, vert[6]);
  glm_mat4_mulv(projViewInv, (vec4){ 1.0f,  1.0f,  1.0f, 1.0f}, vert[7]);

  memset(min, 0, sizeof(vec3));
  memset(max, 0, sizeof(vec3));

  for (i = 0; i < 8; i++) {
    glm_vec4_scale(vert[i], 1.0f / vert[i][3], vert[i]);

    if (vert[i][0] < min[0])
      min[0] = vert[i][0];
    else if (vert[i][0] > max[0])
      max[0] = vert[i][0];


    if (vert[i][1] < min[1])
      min[1] = vert[i][1];
    else if (vert[i][1] > max[1])
      max[1] = vert[i][1];

    if (vert[i][2] < min[2])
      min[2] = vert[i][2];
    else if (vert[i][2] > max[2])
      max[2] = vert[i][2];
  }

  glm_vec_copy(min, cam->bbox.world.vec.min);
  glm_vec_copy(max, cam->bbox.world.vec.max);
}

void
gkCameraProjUpdated(GkCamera * __restrict cam) {
  glm_mat4_mul(cam->proj, cam->view, cam->projView);
  gkPrepareCameraProp(cam);
}

void
gkCameraViewUpdated(GkCamera * __restrict cam) {
  glm_mat4_inv(cam->world, cam->view);
  glm_mat4_mul(cam->proj,  cam->view, cam->projView);
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
