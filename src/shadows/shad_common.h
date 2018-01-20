/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
      GkSpotLight *spot;
      vec3         pos;

      spot = (GkSpotLight*)light;

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
