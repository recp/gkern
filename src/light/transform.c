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
      glm_ortho_aabb(finalBox, proj);
      break;
    }
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;
      vec3         pos;

      spot = (GkSpotLight*)light;

      gkLightPos(scene, light, pos);
      glm_look_anyup(pos, light->dir, view);
      glm_frustum_box(cam->frustum.corners, view, box);

      glm_aabb_transform(boxInFrustum, view, boxInFrustum);
      glm_aabb_crop(box, boxInFrustum, finalBox);

      glm_perspective(spot->falloffAngle,
                      1.0f,
                      glm_max(-finalBox[1][2], 0.0f),
                     -finalBox[0][2],
                      proj);
      break;
    }
    case GK_LIGHT_TYPE_POINT:
      break;
    default:
      break;
  }

  glm_mat4_mul(proj, view, viewProj);
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
    glm_vec3_copy(trans->world[3], position);
    return;
  }

  glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, position);
}

void
gkLightDir(GkScene *scene, GkLight *light, vec3 dir) {
  GkTransform *trans;
  if ((trans = gkLightTransform(light))) {
    GkFinalTransform *ftr;
    GkCamera         *cam;

    cam = scene->camera;
    if ((ftr = gkFinalTransform(trans, cam))) {
      glm_vec3_rotate_m4(ftr->mv, light->dir, dir);
    } else {
      mat4 mv;
      glm_mul(cam->view, trans->world, mv);
      glm_vec3_rotate_m4(mv, light->dir, dir);
    }

    return;
  }

  glm_vec3_copy(light->dir, dir);
}

void
gkLightDirWorld(GkScene *scene, GkLight *light, vec3 dir) {
  GkTransform *trans;
  if ((trans = gkLightTransform(light))) {
    glm_vec3_rotate_m4(trans->world, light->dir, dir);
    return;
  }

  glm_vec3_copy(light->dir, dir);
}

/* todo: cache this */
void
gkLightRotation(GkScene *scene,
                GkLight *light,
                vec3     right,
                vec3     up,
                vec3     fwd) {
  GkTransform *trans;

  glm_vec3_copy(light->dir, fwd);
  glm_vec3_copy((vec3){0.0, 1.0, 0.0f},  up);

  if ((trans = gkLightTransform(light)))
    glm_vec3_rotate_m4(trans->world, up, up);

  glm_vec3_crossn(fwd, up, right);

  glm_vec3_normalize(up);
  glm_vec3_normalize(fwd);
}
