/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"
#include "../../../include/gk/prims/cube.h"
#include "../../default/gk_def_effect.h"
#include "../../default/ak_def_light.h"
#include "../../gk_matrix.h"

#include "rn_light.h"
#include "rn_pass.h"

void
gkRenderPrimPerLight(GkScene     * __restrict scene,
                     GkPrimitive * __restrict prim,
                     GkProgInfo  * __restrict pinfo) {
  GkLight *light;
  GLint    loc;

  light = (GkLight *)scene->lights;
  if (!light) {
    light             = gk_def_lights();
    light->isvalid    = false;
    scene->lightCount = 1;
  }

  do {
    if (!light->isvalid) {
      if (light->node) {
        GkNode           *node;
        GkFinalTransform *ftr;

        node = light->node;
        ftr  = node->trans->ftr;

        gkUniformSingleLight(scene,
                             light,
                             pinfo,
                             ftr->mv);
      } else {
        gkUniformSingleLight(scene,
                             light,
                             pinfo,
                             GLM_MAT4_IDENTITY);
      }
    }

    loc = glGetUniformLocation(pinfo->prog, "lightType");
    glUniform1ui(loc, light->type);

    gkRenderPrim(scene, prim);

    light = (GkLight *)light->ref.next;
  } while (light);

  pinfo->updtLights = 0;
}
