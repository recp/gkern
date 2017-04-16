/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"
#include "../include/gk-camera.h"
#include <stdlib.h>
#include <string.h>

void
gkSetCamera(struct GkScene * __restrict scene,
            GkCamera       * __restrict camera) {
  scene->camera = camera;
  scene->camera->flags = 0;
}

GkCamera*
gkMakeCamera(mat4 proj, mat4 view) {
  GkCamera *cam;

  cam = malloc(sizeof(*cam));

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

  cam = malloc(sizeof(*cam));

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
