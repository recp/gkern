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
  GkLight *light;

  if (!camera)
    return;

  scene->camera = camera;
  scene->camera->flags = 0;

  if ((light = (GkLight *)scene->lights) && !light->node)
    glm_vec_rotate_m4(camera->world,
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
  glm_vec_center(scene->bbox[0], scene->bbox[1], target);

  if (!gkSceneIs2D(scene)) {
    glm_vec_sub(scene->bbox[1], target, eye);
    dist = glm_vec_distance(target, eye);

    glm_vec_sub(scene->bbox[1], target, eye);
    dist = glm_vec_distance(target, eye);

    glm_vec_scale(eye, 2.5f, eye); // TODO: read this as option
    glm_vec_add(target, eye, eye);

    far = glm_vec_distance(eye, scene->bbox[0]) + dist;
  } else {
    far  = glm_aabb_size(scene->bbox);
    dist = 0.01f;

    glm_vec_copy(target, eye);

    if (glm_eq(eye[0], 0.0f))
      eye[0] += far;
    if (glm_eq(eye[1], 0.0f))
      eye[1] += far;
    if (glm_eq(eye[2], 0.0f))
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
  glm_mat4_inv_precise(view, cam->world);

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

  cam->flags = 0;

  return cam;
}

void
gkUpdateCamera(GkCamera *cam, mat4 proj, mat4 view) {
  glm_mat4_copy(proj, cam->proj);
  glm_mat4_copy(view, cam->view);
  glm_mat4_mul(proj, view, cam->viewProj);
  glm_mat4_inv_precise(view, cam->world);

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
  GkCamera     *cam;
  GkCameraImpl *camImpl;
  vec3          dir;

  if (!(cam = scene->camera) || distance == 0.0f)
    return;

  camImpl = (GkCameraImpl *)cam;

  if (isinf(distance) || isnan(distance))
    distance = camImpl->lastZoomDist * copysignf(1.0f, distance);

  glm_vec_scale_as(cam->world[2], -distance, dir);
  glm_vec_add(cam->world[3], dir, cam->world[3]);

  gkCameraViewUpdated(cam);

  scene->camera->flags |= GK_UPDT_VIEW;
  scene->flags |= GK_SCENEF_RENDER;

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
