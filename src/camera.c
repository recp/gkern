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
  GkLight *light;

  if (!camera)
    return;

  scene->camera = camera;
  scene->camera->flags = 0;

  if ((light = (GkLight *)scene->lights) && !light->node)
    glm_vec3_rotate_m4(camera->world,
                      light->defdir,
                      light->dir);
}

GkCamera*
gkMakeCameraForScene(GkScene *scene) {
  vec3  target;
  vec3  eye;
  mat4  view, proj;
  float aspectRatio, far, dist;

  if (!(scene->flags & GK_SCENEF_PREPARED))
    gkPrepareScene(scene);

  aspectRatio = scene->viewport[2] / scene->viewport[3];
  glm_vec3_center(scene->bbox[0], scene->bbox[1], target);

  if (!gkSceneIs2D(scene)) {
    glm_vec3_sub(scene->bbox[1], target, eye);
    dist = glm_vec3_distance(target, eye);

    glm_vec3_scale(eye, 2.5f, eye); // TODO: read this as option
    glm_vec3_add(target, eye, eye);

    far = glm_vec3_distance(eye, scene->bbox[0]) + dist;
  } else {
    far  = glm_aabb_size(scene->bbox) * 2;
    dist = 0.01f;

    glm_vec3_copy(target, eye);

    if (fabsf(scene->bbox[1][0] - scene->bbox[0][0]) <= 0.0001)
      eye[0] += far;
    if (fabsf(scene->bbox[1][1] - scene->bbox[0][1]) <= 0.0001)
      eye[1] += far;
    if (fabsf(scene->bbox[1][2] - scene->bbox[0][2]) <= 0.0001)
      eye[2] += far;

    far *= 2;
  }

  glm_perspective(M_PI_4,
                  aspectRatio,
                  glm_max(dist * 0.01, 0.01),
                  far,
                  proj);

  glm_lookat(eye, target, GLM_YUP, view); // TODO: up axis

  return gkMakeCamera(proj, view);
}

GkCamera*
gkMakeCamera(mat4 proj, mat4 view) {
  GkCamera *cam;

  cam = gkAllocCamera();

  glm_mat4_copy(proj, cam->proj);
  glm_mat4_copy(view, cam->view);
  glm_mat4_mul(proj, view, cam->viewProj);
  glm_mat4_inv(view, cam->world);

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
  glm_mat4_inv(view, cam->view);

  cam->flags = 0;

  return cam;
}

void
gkUpdateCamera(GkCamera *cam, mat4 proj, mat4 view) {
  glm_mat4_copy(proj, cam->proj);
  glm_mat4_copy(view, cam->view);
  glm_mat4_mul(proj, view, cam->viewProj);
  glm_mat4_inv(view, cam->world);

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
  glm_mat4_inv(cam->view,  cam->world);
  glm_mat4_mul(cam->proj,  cam->view, cam->viewProj);
  gkPrepareCameraProp(cam);
}

void
gkCameraWorldUpdated(GkCamera * __restrict cam) {
  glm_mat4_inv(cam->world, cam->view);
  glm_mat4_mul(cam->proj,  cam->view, cam->viewProj);
  gkPrepareCameraProp(cam);
}

void
gkZoom(GkScene * __restrict scene,
       float distance) {
  GkCamera     *cam;
  GkCameraImpl *camImpl;
  vec3          dir;

  if (!(cam = scene->camera) || distance == 0.0f)
    return;

  camImpl = (GkCameraImpl *)cam;

  if (isinf(distance) || isnan(distance))
    distance = camImpl->lastZoomDist * copysignf(1.0f, distance);

  glm_vec3_scale_as(cam->world[2], -distance, dir);
  glm_vec3_add(cam->world[3], dir, cam->world[3]);

  glm_persp_move_far(cam->proj, -distance);
  gkCameraWorldUpdated(cam);

  scene->camera->flags |= GK_UPDT_VIEW;
  scene->flags         |= GK_SCENEF_RENDER;

  camImpl->lastZoomDist = fabsf(distance);
}

void
gkZoomInOneUnit(GkScene * __restrict scene) {
  gkZoom(scene, 1.0f);
}

void
gkZoomOutOneUnit(GkScene * __restrict scene) {
  gkZoom(scene, -1.0f);
}
