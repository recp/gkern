/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include <string.h>

#include "../program/uniform_cache.h"
#include "../default/def_effect.h"
#include "colortex_uniform.h"

void
gkUniformMaterial(struct GkContext  * __restrict ctx,
                  struct GkProgram  * __restrict prog,
                  struct GkMaterial * __restrict material) {
  GkTechnique *technique;
  char         buf[256];
  GLint        loc;
  uint32_t     texUnit;

  if (prog->lastMaterial == material
      && !prog->updtMaterials)
    return;

  /* apply default material */
  if (!material)
    material = gk_def_material();

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "mat.");

  texUnit   = (uint32_t)ctx->samplers->count;
  technique = material->technique;
  if (technique->type == GK_MATERIAL_PHONG
      || technique->type == GK_MATERIAL_BLINN) {
    GkPhong *phong;
    phong  = (GkPhong *)material->technique;

    if (phong->ambient)
      gkUniformColorOrTex(phong->ambient,  buf, "ambient",  prog, &texUnit);
    if (phong->diffuse)
      gkUniformColorOrTex(phong->diffuse,  buf, "diffuse",  prog, &texUnit);
    if (phong->specular)
      gkUniformColorOrTex(phong->specular, buf, "specular", prog, &texUnit);
    if (phong->emission)
      gkUniformColorOrTex(phong->emission, buf, "emission", prog, &texUnit);

    loc = gkUniformLocBuff(prog, "shininess", buf);
    glUniform1f(loc, phong->shininess);
  } else if (technique->type == GK_MATERIAL_LAMBERT) {
    GkLambert *lambert;
    lambert  = (GkLambert *)material->technique;

    if (lambert->ambient)
      gkUniformColorOrTex(lambert->ambient,  buf, "ambient",  prog, &texUnit);
    if (lambert->diffuse)
      gkUniformColorOrTex(lambert->diffuse,  buf, "diffuse",  prog, &texUnit);
    if (lambert->emission)
      gkUniformColorOrTex(lambert->emission, buf, "emission", prog, &texUnit);
  } else if (technique->type == GK_MATERIAL_CONSTANT) {
    GkConstant *constant;
    constant  = (GkConstant *)material->technique;

    if (constant->emission)
      gkUniformColorOrTex(constant->emission, buf, "emission", prog, &texUnit);
  }

  if (material->transparent) {
    GkTransparent *transp;

    transp = material->transparent;
    if (transp->color) {
      gkUniformColorOrTex(transp->color,
                          buf,
                          "transparent",
                          prog,
                          &texUnit);
    } else {
      loc = gkUniformLocBuff(prog, "transparent", buf);
      glUniform4fv(loc, 1, (vec4){1.0f, 1.0f, 1.0f, 1.0f});
    }

    loc = gkUniformLocBuff(prog, "transparency", buf);
    glUniform1f(loc, transp->amount);
  }

  if (material->reflective) {
    GkReflective *refl;
    refl = material->reflective;

    if (refl->color)
      gkUniformColorOrTex(refl->color,
                          buf,
                          "reflective",
                          prog,
                          &texUnit);

    loc = gkUniformLocBuff(prog, "reflectivity", buf);
    glUniform1f(loc, refl->amount);
  }

  loc = gkUniformLocBuff(prog, "indexOfRefraction", buf);
  glUniform1f(loc, material->indexOfRefraction);

  prog->lastMaterial  = material;
  prog->updtMaterials = false;
}
