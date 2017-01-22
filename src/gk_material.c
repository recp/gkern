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
  GkTechnique   *technique;
  GkFinalMatrix *fmat;
  GkProgInfo    *pinfo;
  char           buf[256];
  GLint          loc;
  GLuint         locui;
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

  technique = material->technique;
  if (technique->type == GK_MATERIAL_PHONG
      || technique->type == GK_MATERIAL_BLINN) {
    GkPhong *phong;
    phong  = (GkPhong *)material->technique;

    loc = gkGetUniformLoc(prog, buf, "ambient.color");
    glUniform4fv(loc, 1, phong->ambient.color.vec);

    loc = gkGetUniformLoc(prog, buf, "diffuse.color");
    glUniform4fv(loc, 1, phong->diffuse.color.vec);

    loc = gkGetUniformLoc(prog, buf, "specular.color");
    glUniform4fv(loc, 1, phong->specular.color.vec);

    loc = gkGetUniformLoc(prog, buf, "emission.color");
    glUniform4fv(loc, 1, phong->emission.color.vec);

    loc = gkGetUniformLoc(prog, buf, "reflective.color");
    glUniform4fv(loc, 1, phong->reflective.color.vec);

    loc = gkGetUniformLoc(prog, buf, "transparent.color");
    glUniform4fv(loc, 1, phong->transparent.color.vec);

    loc = gkGetUniformLoc(prog, buf, "shininess");
    glUniform1f(loc, phong->shininess);

    loc = gkGetUniformLoc(prog, buf, "reflectivity");
    glUniform1f(loc, phong->reflectivity);

    loc = gkGetUniformLoc(prog, buf, "transparency");
    glUniform1f(loc, phong->transparency);

    loc = gkGetUniformLoc(prog, buf, "indexOfRefraction");
    glUniform1f(loc, phong->indexOfRefraction);
  } else if (technique->type == GK_MATERIAL_LAMBERT) {
    GkLambert *lambert;
    lambert  = (GkLambert *)material->technique;

    loc = gkGetUniformLoc(prog, buf, "ambient.color");
    glUniform4fv(loc, 1, lambert->ambient.color.vec);

    loc = gkGetUniformLoc(prog, buf, "diffuse.color");
    glUniform4fv(loc, 1, lambert->diffuse.color.vec);

    loc = gkGetUniformLoc(prog, buf, "emission.color");
    glUniform4fv(loc, 1, lambert->emission.color.vec);

    loc = gkGetUniformLoc(prog, buf, "reflective.color");
    glUniform4fv(loc, 1, lambert->reflective.color.vec);

    loc = gkGetUniformLoc(prog, buf, "transparent.color");
    glUniform4fv(loc, 1, lambert->transparent.color.vec);

    loc = gkGetUniformLoc(prog, buf, "reflectivity");
    glUniform1f(loc, lambert->reflectivity);

    loc = gkGetUniformLoc(prog, buf, "transparency");
    glUniform1f(loc, lambert->transparency);

    loc = gkGetUniformLoc(prog, buf, "indexOfRefraction");
    glUniform1f(loc, lambert->indexOfRefraction);
  } else if (technique->type == GK_MATERIAL_CONSTANT) {
    GkConstant *constant;
    constant  = (GkConstant *)material->technique;

    loc = gkGetUniformLoc(prog, buf, "emission.color");
    glUniform4fv(loc, 1, constant->emission.color.vec);

    loc = gkGetUniformLoc(prog, buf, "reflective.color");
    glUniform4fv(loc, 1, constant->reflective.color.vec);

    loc = gkGetUniformLoc(prog, buf, "transparent.color");
    glUniform4fv(loc, 1, constant->transparent.color.vec);

    loc = gkGetUniformLoc(prog, buf, "reflectivity");
    glUniform1f(loc, constant->reflectivity);

    loc = gkGetUniformLoc(prog, buf, "transparency");
    glUniform1f(loc, constant->transparency);

    loc = gkGetUniformLoc(prog, buf, "indexOfRefraction");
    glUniform1f(loc, constant->indexOfRefraction);
  }


  locui = glGetSubroutineIndex(prog,
                               GL_FRAGMENT_SHADER,
                               technique->subroutine);

  glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &locui);
}
