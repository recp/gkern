/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "colortex_uniform.h"

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include "../program/uniform_cache.h"
#include "../default/def_effect.h"
#include "../state/gpu.h"

#include <ds/forward-list.h>
#include <string.h>

void
gkUniformMaterialStruct(struct GkContext  * __restrict ctx,
                        struct GkProgram  * __restrict prog,
                        struct GkMaterial * __restrict mat) {
  GkTechnique *technique;
  char         buf[256];
  GLint        loc;

  /* only re-bind textures if needed */
  if (prog->lastMaterial == mat
      && !prog->updtMaterials) {
    FListItem *itm;
    GkTexture *tex;

    itm = mat->boundTextures;
    while (itm) {
      tex = itm->data;
      gkBindTextureTo(ctx, tex->boundUnit, tex->target, tex->index);
      itm = itm->next;
    }

    return;
  }

  /* apply default material */
  if (!mat)
    mat = gk_def_material();

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "mat.");

  /* reset tex unit for this material */
  ctx->availTexUnit = (uint32_t)ctx->samplers->count;
  technique         = mat->technique;

  if (technique->type == GK_MATERIAL_PHONG
      || technique->type == GK_MATERIAL_BLINN) {
    GkPhong *phong;
    phong  = (GkPhong *)mat->technique;

    if (phong->ambient)
      gkUniformColorOrTexBuff(ctx, mat, phong->ambient,  buf, "ambient",  prog);
    if (phong->diffuse)
      gkUniformColorOrTexBuff(ctx, mat, phong->diffuse,  buf, "diffuse",  prog);
    if (phong->specular)
      gkUniformColorOrTexBuff(ctx, mat, phong->specular, buf, "specular", prog);
    if (phong->emission)
      gkUniformColorOrTexBuff(ctx, mat, phong->emission, buf, "emission", prog);

    loc = gkUniformLocBuff(prog, "shininess", buf);
    glUniform1f(loc, phong->shininess);
  } else if (technique->type == GK_MATERIAL_LAMBERT) {
    GkLambert *lambert;
    lambert  = (GkLambert *)mat->technique;

    if (lambert->ambient)
      gkUniformColorOrTexBuff(ctx, mat, lambert->ambient,  buf, "ambient",  prog);
    if (lambert->diffuse)
      gkUniformColorOrTexBuff(ctx, mat, lambert->diffuse,  buf, "diffuse",  prog);
    if (lambert->emission)
      gkUniformColorOrTexBuff(ctx, mat, lambert->emission, buf, "emission", prog);
  } else if (technique->type == GK_MATERIAL_CONSTANT) {
    GkConstant *constant;
    constant  = (GkConstant *)mat->technique;

    if (constant->emission)
      gkUniformColorOrTexBuff(ctx, mat, constant->emission, buf, "emission", prog);
  }

  if (mat->transparent) {
    GkTransparent *transp;

    transp = mat->transparent;
    if (transp->color) {
      gkUniformColorOrTexBuff(ctx,
                              mat,
                              transp->color,
                              buf,
                              "transparent",
                              prog);
    } else {
      loc = gkUniformLocBuff(prog, "transparent", buf);
      glUniform4fv(loc, 1, (vec4){1.0f, 1.0f, 1.0f, 1.0f});
    }

    loc = gkUniformLocBuff(prog, "transparency", buf);
    glUniform1f(loc, transp->amount);
  }

  if (mat->reflective) {
    GkReflective *refl;
    refl = mat->reflective;

    if (refl->color)
      gkUniformColorOrTexBuff(ctx,
                              mat,
                              refl->color,
                              buf,
                              "reflective",
                              prog);

    loc = gkUniformLocBuff(prog, "reflectivity", buf);
    glUniform1f(loc, refl->amount);
  }

  loc = gkUniformLocBuff(prog, "indexOfRefraction", buf);
  glUniform1f(loc, mat->indexOfRefraction);

  prog->lastMaterial  = mat;
  prog->updtMaterials = false;
}

void
gkUniformMaterial(struct GkContext  * __restrict ctx,
                  struct GkProgram  * __restrict prog,
                  struct GkMaterial * __restrict mat) {
  GkTechnique *technique;
  GLint        loc;

  /* only re-bind textures if needed */
  if (prog->lastMaterial == mat
      && !prog->updtMaterials) {
    FListItem *itm;
    GkTexture *tex;

    itm = mat->boundTextures;
    while (itm) {
      tex = itm->data;
      gkBindTextureTo(ctx, tex->boundUnit, tex->target, tex->index);
      itm = itm->next;
    }

    return;
  }

  /* apply default material */
  if (!mat)
    mat = gk_def_material();

  flist_sp_destroy(&mat->boundTextures);

  /* reset tex unit for this material */
  ctx->availTexUnit = (uint32_t)ctx->samplers->count;
  technique         = mat->technique;

  if (technique->type == GK_MATERIAL_PHONG
      || technique->type == GK_MATERIAL_BLINN) {
    GkPhong *phong;
    phong  = (GkPhong *)mat->technique;
    
    if (phong->ambient)
      gkUniformColorOrTex(ctx, mat, phong->ambient,  "uAmbient",  prog);
    if (phong->diffuse)
      gkUniformColorOrTex(ctx, mat, phong->diffuse,  "uDiffuse",  prog);
    if (phong->specular)
      gkUniformColorOrTex(ctx, mat, phong->specular, "uSpecular", prog);
    if (phong->emission)
      gkUniformColorOrTex(ctx, mat, phong->emission, "uEmission", prog);
    
    loc = gkUniformLoc(prog, "uShininess");
    glUniform1f(loc, phong->shininess);
  } else if (technique->type == GK_MATERIAL_LAMBERT) {
    GkLambert *lambert;
    lambert  = (GkLambert *)mat->technique;
    
    if (lambert->ambient)
      gkUniformColorOrTex(ctx, mat, lambert->ambient, "uAmbient",  prog);
    if (lambert->diffuse)
      gkUniformColorOrTex(ctx, mat, lambert->diffuse, "uDiffuse",  prog);
    if (lambert->emission)
      gkUniformColorOrTex(ctx, mat, lambert->emission, "uSpecular", prog);
  } else if (technique->type == GK_MATERIAL_CONSTANT) {
    GkConstant *constant;
    constant  = (GkConstant *)mat->technique;
    
    if (constant->emission)
      gkUniformColorOrTex(ctx, mat, constant->emission, "uEmission", prog);
  }
  
  if (mat->transparent) {
    GkTransparent *transp;
    
    transp = mat->transparent;
    if (transp->color) {
      gkUniformColorOrTex(ctx, mat, transp->color, "uTransparent", prog);
    } else {
      loc = gkUniformLoc(prog, "uTransparent");
      glUniform4fv(loc, 1, (vec4){1.0f, 1.0f, 1.0f, 1.0f});
    }
    
    loc = gkUniformLoc(prog, "uTransparency");
    glUniform1f(loc, transp->amount);
  }
  
  if (mat->reflective) {
    GkReflective *refl;
    refl = mat->reflective;
    
    if (refl->color)
      gkUniformColorOrTex(ctx,
                          mat,
                          refl->color,
                          "uReflective",
                          prog);
    
    loc = gkUniformLoc(prog, "uReflectivity");
    glUniform1f(loc, refl->amount);
  }
  
  loc = gkUniformLoc(prog, "uIndexOfRefraction");
  glUniform1f(loc, mat->indexOfRefraction);
  
  prog->lastMaterial  = mat;
  prog->updtMaterials = false;
}
