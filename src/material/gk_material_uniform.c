/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include <string.h>

#include "../gk_uniform.h"
#include "../default/gk_def_effect.h"
#include "gk_colortex_uniform.h"

void
gkUniformMaterial(struct GkProgInfo * __restrict pinfo,
                  struct GkMaterial * __restrict material) {
  GkTechnique   *technique;
  char           buf[256];
  GLint          loc;
  GLuint         locui;
  GLuint         prog;

  if (pinfo->lastMaterial == material
      && !pinfo->updtMaterials)
    return;

  /* apply default material */
  if (!material)
    material = gk_def_material();

  prog = pinfo->prog;

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "material.");

  technique = material->technique;
  if (technique->type == GK_MATERIAL_PHONG
      || technique->type == GK_MATERIAL_BLINN) {
    GkPhong *phong;
    phong  = (GkPhong *)material->technique;

    if (phong->ambient)
      gkUniformColorOrTex(phong->ambient,     buf, "ambient",     prog);
    if (phong->diffuse)
      gkUniformColorOrTex(phong->diffuse,     buf, "diffuse",     prog);
    if (phong->specular)
      gkUniformColorOrTex(phong->specular,    buf, "specular",    prog);
    if (phong->emission)
      gkUniformColorOrTex(phong->emission,    buf, "emission",    prog);
    if (phong->reflective)
      gkUniformColorOrTex(phong->reflective,  buf, "reflective",  prog);
    if (phong->transparent)
      gkUniformColorOrTex(phong->transparent, buf, "transparent", prog);

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

    if (lambert->ambient)
      gkUniformColorOrTex(lambert->ambient,     buf, "ambient",     prog);
    if (lambert->diffuse)
      gkUniformColorOrTex(lambert->diffuse,     buf, "diffuse",     prog);
    if (lambert->emission)
      gkUniformColorOrTex(lambert->emission,    buf, "emission",    prog);
    if (lambert->reflective)
      gkUniformColorOrTex(lambert->reflective,  buf, "reflective",  prog);
    if (lambert->transparent)
      gkUniformColorOrTex(lambert->transparent, buf, "transparent", prog);

    loc = gkGetUniformLoc(prog, buf, "reflectivity");
    glUniform1f(loc, lambert->reflectivity);

    loc = gkGetUniformLoc(prog, buf, "transparency");
    glUniform1f(loc, lambert->transparency);

    loc = gkGetUniformLoc(prog, buf, "indexOfRefraction");
    glUniform1f(loc, lambert->indexOfRefraction);
  } else if (technique->type == GK_MATERIAL_CONSTANT) {
    GkConstant *constant;
    constant  = (GkConstant *)material->technique;

    if (constant->emission)
      gkUniformColorOrTex(constant->emission,    buf, "emission",    prog);
    if (constant->reflective)
      gkUniformColorOrTex(constant->reflective,  buf, "reflective",  prog);
    if (constant->transparent)
      gkUniformColorOrTex(constant->transparent, buf, "transparent", prog);

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

  pinfo->lastMaterial  = material;
  pinfo->updtMaterials = false;
}
