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

GkCamera*
gkCameraOfLight(GkScene *scene, GkLight *light) {
  mat4      view, proj;
  GkCamera *cam;

  cam = scene->camera;

  switch (light->type) {
    case GK_LIGHT_TYPE_DIRECTIONAL: {
      vec3  dir, pos;
      float r;

      r = cam->bbox.radius;
      gkLightDirWorld(scene, light, dir);
      glm_vec_normalize(dir);

      glm_vec_scale(dir, r, pos);
      glm_vec_sub(cam->bbox.center, pos, pos);

      glm_lookat(pos, cam->bbox.center, GLM_YUP, view);

      glm_ortho(-r, r, -r, r, -r, r, proj);
      break;
    }
    case GK_LIGHT_TYPE_POINT:
    case GK_LIGHT_TYPE_SPOT:

    default:
      break;
  }

  return (light->camera = gkMakeCamera(proj, view));
}

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

void
gkLightDirWorld(GkScene *scene, GkLight *light, vec3 dir) {
  GkTransform *trans;
  if ((trans = gkLightTransform(light))) {
    glm_vec_rotate_m4(trans->world, light->direction, dir);
    return;
  }

  glm_vec_copy(light->direction, dir);
}

