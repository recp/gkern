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
gkShadowMatrix(struct GkScene *scene,
               GkLight        *light,
               mat4            viewProj) {
  mat4       view, proj;
  GkCamera  *cam;
  GkFrustum *frustum;
  vec3       box[2], boxInFrustum[2], finalBox[2];

  cam     = scene->camera;
  frustum = &cam->frustum;

  gkBoxInFrustum(frustum, boxInFrustum);

  switch (light->type) {
    case GK_LIGHT_TYPE_DIRECTIONAL: {
      glm_look_anyup(cam->frustum.center, light->dir, view);
      glm_frustum_box(cam->frustum.corners, view, box);

      glm_aabb_transform(boxInFrustum, view, boxInFrustum);
      glm_aabb_crop(box, boxInFrustum, finalBox);
      glm_ortho_box(finalBox, proj);

      glm_mat4_mul(proj, view, viewProj);
      break;
    }
    case GK_LIGHT_TYPE_POINT:
    case GK_LIGHT_TYPE_SPOT:

    default:
      break;
  }
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
