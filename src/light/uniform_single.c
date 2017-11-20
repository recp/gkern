/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../default/def_light.h"
#include "../program/uniform_cache.h"
#include "matrix.h"
#include <string.h>

void
gkUniformSingleLight(struct GkScene * __restrict scene,
                     GkLight        * __restrict light,
                     GkProgram      * __restrict prog) {
  vec4 *transView;
  vec4  dir;
  char  buf[32];
  GLint loc;
  
  transView = NULL;
  if (light->node) {
    GkNode           *node;
    GkFinalTransform *ftr;
    
    node = light->node;
    ftr  = gkFinalTransform(node->trans, scene->camera);
    
    transView = ftr->mv;
  }

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "light.");

  switch (light->type) {
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;

      spot = (GkSpotLight *)light;

      loc = gkUniformLocBuff(prog, "cutoffCos", buf);
      glUniform1f(loc, spot->cutoffCosine);

      loc = gkUniformLocBuff(prog, "cutoffExp", buf);
      glUniform1f(loc, spot->cutoffExp);

      loc = gkUniformLocBuff(prog, "constAttn", buf);
      glUniform1f(loc, spot->constAttn);

      loc = gkUniformLocBuff(prog, "linAttn", buf);
      glUniform1f(loc, spot->linearAttn);

      loc = gkUniformLocBuff(prog, "quadAttn", buf);
      glUniform1f(loc, spot->quadAttn);
      break;
    }

    case GK_LIGHT_TYPE_POINT: {
      GkPointLight *point;

      point = (GkPointLight *)light;

      loc = gkUniformLocBuff(prog, "constAttn", buf);
      glUniform1f(loc, point->constAttn);

      loc = gkUniformLocBuff(prog, "linAttn", buf);
      glUniform1f(loc, point->linearAttn);

      loc = gkUniformLocBuff(prog, "quadAttn", buf);
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
    loc = gkUniformLocBuff(prog, "ambient", buf);
    glUniform4fv(loc, 1, *light->ambient);
  }

  loc = gkUniformLocBuff(prog, "color", buf);
  glUniform4fv(loc, 1, light->color.vec);

  loc = gkUniformLocBuff(prog, "position", buf);

  /* light/cone direction */
  if (transView) {
    /* position must be in view space */
    glUniform3fv(loc, 1, transView[3]);
    glm_vec_rotate_m4(transView, light->direction, dir);
  } else {
    /* position must be in view space */
    glUniform3fv(loc, 1, (vec3){0.0f, 0.0f, 0.0f});
  }

  loc = gkUniformLocBuff(prog, "direction", buf);
  glUniform3fv(loc, 1, dir);
  
  gkUniform1ui(prog, "lightType", light->type);
  prog->lastLight = light;
}
