/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../default/def_light.h"
#include "../program/uniform_cache.h"
#include "matrix.h"
#include <string.h>

void
gkUniformSingleLight(struct GkScene * __restrict scene,
                     GkLight        * __restrict light,
                     GkProgInfo     * __restrict pinfo,
                     mat4                        transView) {
  vec4  dir;
  char  buf[32];
  GLint loc;

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "light.");

  switch (light->type) {
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;

      spot = (GkSpotLight *)light;

      loc = gkUniformLocBuff(pinfo, "cutoffCos", buf);
      glUniform1f(loc, spot->cutoffCosine);

      loc = gkUniformLocBuff(pinfo, "cutoffExp", buf);
      glUniform1f(loc, spot->cutoffExp);

      loc = gkUniformLocBuff(pinfo, "constAttn", buf);
      glUniform1f(loc, spot->constAttn);

      loc = gkUniformLocBuff(pinfo, "linAttn", buf);
      glUniform1f(loc, spot->linearAttn);

      loc = gkUniformLocBuff(pinfo, "quadAttn", buf);
      glUniform1f(loc, spot->quadAttn);
      break;
    }

    case GK_LIGHT_TYPE_POINT: {
      GkPointLight *point;

      point = (GkPointLight *)light;

      loc = gkUniformLocBuff(pinfo, "constAttn", buf);
      glUniform1f(loc, point->constAttn);

      loc = gkUniformLocBuff(pinfo, "linAttn", buf);
      glUniform1f(loc, point->linearAttn);

      loc = gkUniformLocBuff(pinfo, "quadAttn", buf);
      glUniform1f(loc, point->quadAttn);
      break;
    }
    case GK_LIGHT_TYPE_DIRECTIONAL:
    case GK_LIGHT_TYPE_AMBIENT:
      break;
    default:
      printf("TODO: custom lights currently are unsupported\n");
      return;
  }

  if (light->ambient) {
    loc = gkUniformLocBuff(pinfo, "ambient", buf);
    glUniform4fv(loc, 1, *light->ambient);
  }

  loc = gkUniformLocBuff(pinfo, "color", buf);
  glUniform4fv(loc, 1, light->color.vec);

  loc = gkUniformLocBuff(pinfo, "position", buf);

  /* position must be in view space */
  glUniform3fv(loc, 1, transView[3]);

  /* light/cone direction */
  glm_vec_rotate_m4(transView,
                    light->direction,
                    dir);

  loc = gkUniformLocBuff(pinfo, "direction", buf);
  glUniform3fv(loc, 1, dir);
}
