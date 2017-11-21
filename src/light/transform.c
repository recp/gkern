/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/light.h"
#include <string.h>

GkTransform*
gkLightTransform(GkLight * __restrict light) {
  if (light->node)
    return light->node->trans;

  return NULL;
}

void
gkLightPos(GkScene *scene, GkLight *light, vec3 position) {
  GkTransform *trans;
  if ((trans = gkLightTransform(light))) {
    glm_vec_copy(trans->world[3], position);
    return;
  }

  glm_vec_copy((vec3){0.0f, 0.0f, 0.0f}, position);
}

void
gkLightDir(GkScene *scene, GkLight *light, vec3 dir) {
  GkTransform *trans;
  if ((trans = gkLightTransform(light))) {
    GkFinalTransform *ftr;
    GkCamera         *cam;

    cam = scene->camera;
    if ((ftr = gkFinalTransform(trans, cam))) {
      glm_vec_rotate_m4(ftr->mv, light->direction, dir);
    } else {
      mat4 mv;
      glm_mul(cam->view, trans->world, mv);
      glm_vec_rotate_m4(mv, light->direction, dir);
    }

    return;
  }

  glm_vec_copy(light->direction, dir);
}
