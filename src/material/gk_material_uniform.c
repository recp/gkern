/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk.h"
#include "../../include/gk-material.h"
#include <string.h>

#include "../gk_uniform.h"
#include "../default/gk_def_effect.h"
#include "gk_colortex_uniform.h"

void
gkUniformMaterial(struct GkScene     * __restrict scene,
                  struct GkModelInst * __restrict modelInst) {
  GkMaterial    *material;
  GkTechnique   *technique;
  GkProgInfo    *pinfo;
  char           buf[256];
  GLint          loc;
  GLuint         locui;
  GLuint         prog;

  material = modelInst->material;
  if (!material)
    material = modelInst->model->material;

  /* apply default material */
  if (!material)
    material = gk_def_material();

  pinfo = modelInst->model->pinfo;
  prog  = pinfo->prog;

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "material.");

  technique = material->technique;
  if (technique->type == GK_MATERIAL_PHONG
      || technique->type == GK_MATERIAL_BLINN) {
    GkPhong *phong;
    phong  = (GkPhong *)material->technique;

    gkUniformColorOrTex(&phong->ambient,     buf, "ambient",     prog);
    gkUniformColorOrTex(&phong->diffuse,     buf, "diffuse",     prog);
    gkUniformColorOrTex(&phong->specular,    buf, "specular",    prog);
    gkUniformColorOrTex(&phong->emission,    buf, "emission",    prog);
    gkUniformColorOrTex(&phong->reflective,  buf, "reflective",  prog);
    gkUniformColorOrTex(&phong->transparent, buf, "transparent", prog);

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

    gkUniformColorOrTex(&lambert->ambient,     buf, "ambient",     prog);
    gkUniformColorOrTex(&lambert->diffuse,     buf, "diffuse",     prog);
    gkUniformColorOrTex(&lambert->emission,    buf, "emission",    prog);
    gkUniformColorOrTex(&lambert->reflective,  buf, "reflective",  prog);
    gkUniformColorOrTex(&lambert->transparent, buf, "transparent", prog);

    loc = gkGetUniformLoc(prog, buf, "reflectivity");
    glUniform1f(loc, lambert->reflectivity);

    loc = gkGetUniformLoc(prog, buf, "transparency");
    glUniform1f(loc, lambert->transparency);

    loc = gkGetUniformLoc(prog, buf, "indexOfRefraction");
    glUniform1f(loc, lambert->indexOfRefraction);
  } else if (technique->type == GK_MATERIAL_CONSTANT) {
    GkConstant *constant;
    constant  = (GkConstant *)material->technique;

    gkUniformColorOrTex(&constant->emission,    buf, "emission",    prog);
    gkUniformColorOrTex(&constant->reflective,  buf, "reflective",  prog);
    gkUniformColorOrTex(&constant->transparent, buf, "transparent", prog);

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
