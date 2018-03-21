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
      gkUniformColorDescBuff(ctx, mat, phong->ambient,  buf, "ambient",  prog);
    if (phong->diffuse)
      gkUniformColorDescBuff(ctx, mat, phong->diffuse,  buf, "diffuse",  prog);
    if (phong->specular)
      gkUniformColorDescBuff(ctx, mat, phong->specular, buf, "specular", prog);
    if (phong->emission)
      gkUniformColorDescBuff(ctx, mat, phong->emission, buf, "emission", prog);

    loc = gkUniformLocBuff(prog, "shininess", buf);
    glUniform1f(loc, phong->shininess);
  } else if (technique->type == GK_MATERIAL_LAMBERT) {
    GkLambert *lambert;
    lambert  = (GkLambert *)mat->technique;

    if (lambert->ambient)
      gkUniformColorDescBuff(ctx, mat, lambert->ambient,  buf, "ambient",  prog);
    if (lambert->diffuse)
      gkUniformColorDescBuff(ctx, mat, lambert->diffuse,  buf, "diffuse",  prog);
    if (lambert->emission)
      gkUniformColorDescBuff(ctx, mat, lambert->emission, buf, "emission", prog);
  } else if (technique->type == GK_MATERIAL_CONSTANT) {
    GkConstant *constant;
    constant  = (GkConstant *)mat->technique;

    if (constant->emission)
      gkUniformColorDescBuff(ctx, mat, constant->emission, buf, "emission", prog);
  }

  if (mat->transparent) {
    GkTransparent *transp;

    transp = mat->transparent;
    if (transp->color) {
      gkUniformColorDescBuff(ctx,
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
      gkUniformColorDescBuff(ctx,
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
      gkUniformColorDesc(ctx, mat, phong->ambient,  "uAmbient",  prog);
    if (phong->diffuse)
      gkUniformColorDesc(ctx, mat, phong->diffuse,  "uDiffuse",  prog);
    if (phong->specular)
      gkUniformColorDesc(ctx, mat, phong->specular, "uSpecular", prog);
    if (phong->emission)
      gkUniformColorDesc(ctx, mat, phong->emission, "uEmission", prog);
    
    loc = gkUniformLoc(prog, "uShininess");
    glUniform1f(loc, phong->shininess);
  } else if (technique->type == GK_MATERIAL_LAMBERT) {
    GkLambert *lambert;
    lambert  = (GkLambert *)mat->technique;
    
    if (lambert->ambient)
      gkUniformColorDesc(ctx, mat, lambert->ambient, "uAmbient",  prog);
    if (lambert->diffuse)
      gkUniformColorDesc(ctx, mat, lambert->diffuse, "uDiffuse",  prog);
    if (lambert->emission)
      gkUniformColorDesc(ctx, mat, lambert->emission, "uSpecular", prog);
  } else if (technique->type == GK_MATERIAL_CONSTANT) {
    GkConstant *constant;
    constant  = (GkConstant *)mat->technique;
    
    if (constant->emission)
      gkUniformColorDesc(ctx, mat, constant->emission, "uEmission", prog);
  } else if (technique->type == GK_MATERIAL_METALROUGH) {
    GkMetalRough *metalRough;
    metalRough  = (GkMetalRough *)mat->technique;

    gkUniformColor(&metalRough->albedo, "uAlbedo", prog);

    gkUniform1f(prog, "uMetallic",  metalRough->metallic);
    gkUniform1f(prog, "uRoughness", metalRough->roughness);

    if (metalRough->albedoMap)
      gkUniformTex(ctx, mat, metalRough->albedoMap, "uAlbedoTex", prog);

    if (metalRough->metalRoughMap)
      gkUniformTex(ctx, mat, metalRough->metalRoughMap, "uMetalRoughTex", prog);
  }
  
  if (mat->transparent) {
    GkTransparent *transp;
    
    transp = mat->transparent;
    if (transp->color) {
      gkUniformColorDesc(ctx, mat, transp->color, "uTransparent", prog);
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
      gkUniformColorDesc(ctx,
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
