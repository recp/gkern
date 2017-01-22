/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"
#include "../include/gk-material.h"
#include <string.h>

#include "gk_uniform.h"

void
gkUniformMaterial(struct GkScene     * __restrict scene,
                  struct GkModelInst * __restrict modelInst) {
  GkMaterial    *material;
  GkFinalMatrix *fmat;
  GkProgInfo    *pinfo;
  char           buf[256];
  GLint          loc;
  GLuint         prog;

  material = modelInst->material;
  if (!material)
    material = modelInst->model->material;
  if (!material)
    return;

  pinfo = modelInst->model->pinfo;
  fmat  = modelInst->matrix->fmat;
  prog  = pinfo->prog;

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "material.");

  if (material->technique->type == GK_MATERIAL_PHONG) {
    GkPhong *phong;
    phong  = (GkPhong *)material->technique;

    loc = gkGetUniformLoc(prog, buf, "ambient.color");
    glUniform3fv(loc, 1, phong->ambient.color.vec);

    loc = gkGetUniformLoc(prog, buf, "diffuse.color");
    glUniform3fv(loc, 1, phong->diffuse.color.vec);

    loc = gkGetUniformLoc(prog, buf, "specular.color");
    glUniform3fv(loc, 1, phong->specular.color.vec);

    loc = gkGetUniformLoc(prog, buf, "emission.color");
    glUniform3fv(loc, 1, phong->emission.color.vec);

    loc = gkGetUniformLoc(prog, buf, "reflective.color");
    glUniform3fv(loc, 1, phong->reflective.color.vec);

    loc = gkGetUniformLoc(prog, buf, "transparent.color");
    glUniform3fv(loc, 1, phong->transparent.color.vec);

    loc = gkGetUniformLoc(prog, buf, "shininess");
    glUniform1f(loc, phong->shininess);

    loc = gkGetUniformLoc(prog, buf, "reflectivity");
    glUniform1f(loc, phong->reflectivity);

    loc = gkGetUniformLoc(prog, buf, "transparent");
    glUniform1f(loc, phong->transparency);

    loc = gkGetUniformLoc(prog, buf, "indexOfRefraction");
    glUniform1f(loc, phong->indexOfRefraction);
  }
}
