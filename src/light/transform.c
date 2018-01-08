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

void
gkTransformsForLight(struct GkScene *scene,
                     GkLight        *light,
                     mat4           *viewProj,
                     int             splitCount) {
  mat4      view, proj;
  GkCamera *cam;

  cam = scene->camera;

  switch (light->type) {
    case GK_LIGHT_TYPE_DIRECTIONAL: {
      vec4   *corner, v;
      vec3    box[2], target;
      int32_t i;

      memset(box, 0, sizeof(box));

      glm_vec_add(cam->frustum.center, light->dir, target);
      glm_lookat(cam->frustum.center, target, GLM_YUP, view);

      corner = cam->frustum.corners;
      for (i = 0; i < 8; i++) {
        glm_mat4_mulv(view, corner[i], v);

        box[0][0] = glm_min(box[0][0], v[0]);
        box[0][1] = glm_min(box[0][1], v[1]);
        box[0][2] = glm_min(box[0][2], v[2]);

        box[1][0] = glm_max(box[1][0], v[0]);
        box[1][1] = glm_max(box[1][1], v[1]);
        box[1][2] = glm_max(box[1][2], v[2]);
      }

      glm_ortho(box[0][0], box[1][0],
                box[0][1], box[1][1],
                box[0][2], box[1][2],
                proj);
      break;
    }
    case GK_LIGHT_TYPE_POINT:
    case GK_LIGHT_TYPE_SPOT:

    default:
      break;
  }

  glm_mat4_mul(proj, view, viewProj[0]);
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
      glm_vec_rotate_m4(ftr->mv, light->dir, dir);
    } else {
      mat4 mv;
      glm_mul(cam->view, trans->world, mv);
      glm_vec_rotate_m4(mv, light->dir, dir);
    }

    return;
  }

  glm_vec_copy(light->dir, dir);
}

void
gkLightDirWorld(GkScene *scene, GkLight *light, vec3 dir) {
  GkTransform *trans;
  if ((trans = gkLightTransform(light))) {
    glm_vec_rotate_m4(trans->world, light->dir, dir);
    return;
  }

  glm_vec_copy(light->dir, dir);
}

/* todo: cache this */
void
gkLightRotation(GkScene *scene,
                GkLight *light,
                vec3     right,
                vec3     up,
                vec3     fwd) {
  GkTransform *trans;

  glm_vec_copy(light->dir, fwd);
  glm_vec_copy((vec3){0.0, 1.0, 0.0f},  up);

  if ((trans = gkLightTransform(light)))
    glm_vec_rotate_m4(trans->world, up, up);

  glm_vec_cross(fwd, up, right);

  glm_vec_normalize(right);
  glm_vec_normalize(up);
  glm_vec_normalize(fwd);
}
