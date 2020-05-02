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

#ifndef src_shad_common_h
#define src_shad_common_h

#include "../../include/gk/gk.h"
#include "../../include/gk/shadows.h"

GK_INLINE
void
gkShadowViewMatrix(GkScene   *scene,
                   GkLight   *light,
                   GkFrustum *frustum,
                   mat4       view) {
  switch (light->type) {
    case GK_LIGHT_TYPE_DIRECTIONAL: {
      glm_look_anyup(frustum->center, light->dir, view);
      break;
    }
    case GK_LIGHT_TYPE_SPOT: {
      vec3 pos;
      gkLightPos(scene, light, pos);
      glm_look_anyup(pos, light->dir, view);
    }
    case GK_LIGHT_TYPE_POINT:

    default:
      break;
  }
}

GK_INLINE
void
gkShadowProjMatrix(GkLight        *light,
                   GkFrustum      *frustum,
                   mat4            view,
                   float           zPadding,
                   mat4            proj) {
  vec3 box[2], boxInFrustum[2], finalBox[2];

  gkBoxInFrustum(frustum, boxInFrustum);
  glm_frustum_box(frustum->corners, view, box);
  glm_aabb_transform(boxInFrustum, view, boxInFrustum);
  glm_aabb_crop(box, boxInFrustum, finalBox);

  switch (light->type) {
    case GK_LIGHT_TYPE_DIRECTIONAL: {
      if (zPadding > 0.0f)
        glm_ortho_aabb_pz(finalBox, zPadding, proj);
      else
        glm_ortho_aabb(finalBox, proj);
      break;
    }
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;
      spot = (GkSpotLight*)light;
      glm_perspective(spot->falloffAngle,
                      1.0f,
                      glm_max(-finalBox[1][2], 0.0f),
                     -finalBox[0][2],
                      proj);
    }
    case GK_LIGHT_TYPE_POINT:

    default:
      break;
  }
}

#endif /* src_shad_common_h */
