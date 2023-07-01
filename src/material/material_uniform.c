/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
                        struct GkPipeline  * __restrict prog,
                        struct GkMaterial * __restrict mat) {
  GkTechnique *techn;
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
  techn         = mat->technique;

  if (techn->type == GK_MATERIAL_PHONG
      || techn->type == GK_MATERIAL_BLINN) {
    loc = gkUniformLocBuff(prog, "shininess", buf);
    glUniform1f(loc, techn->shininess);
  }

  if (techn->ambient)
    gkUniformColorDescBuff(ctx, mat, techn->ambient,  buf, "ambient",  prog);

  if (techn->diffuse)
    gkUniformColorDescBuff(ctx, mat, techn->diffuse,  buf, "diffuse",  prog);

  if (techn->specular)
    gkUniformColorDescBuff(ctx, mat, techn->specular, buf, "specular", prog);

  if (techn->emission)
    gkUniformColorDescBuff(ctx, mat, techn->emission, buf, "emission", prog);

  if (techn->transparent) {
    GkTransparent *transp;

    transp = techn->transparent;
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

  if (techn->reflective) {
    GkReflective *refl;
    refl = techn->reflective;

    if (refl->color)
      gkUniformColorDescBuff(ctx, mat, refl->color, buf, "reflective", prog);

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
                  struct GkPipeline  * __restrict prog,
                  struct GkMaterial * __restrict mat) {
  GkTechnique *techn;
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
  techn         = mat->technique;

  if (techn->type == GK_MATERIAL_PHONG
      || techn->type == GK_MATERIAL_BLINN) {
    loc = gkUniformLoc(prog, "uShininess");
    glUniform1f(loc, techn->shininess);
  } else if (techn->type == GK_MATERIAL_METALROUGH) {
    GkMetalRough *metalRough;
    float         mr[2];

    metalRough = (GkMetalRough *)mat->technique;
    mr[0]      = metalRough->metallic;
    mr[1]      = metalRough->roughness;

    gkUniform2f(prog, "uMetalRough", mr);
    gkUniformColor(&metalRough->albedo, "uAlbedo", prog);

    if (metalRough->albedoMap)
      gkUniformTex(ctx, mat, metalRough->albedoMap, "uAlbedo", prog);

    if (metalRough->metalRoughMap)
      gkUniformTex(ctx, mat, metalRough->metalRoughMap, "uMetalRough", prog);
  } else if (techn->type == GK_MATERIAL_SPECGLOSS) {
    GkSpecGloss *specGloss;

    specGloss = (GkSpecGloss *)mat->technique;

    gkUniform1f(prog, "uGloss", specGloss->gloss);
    gkUniformColor(&specGloss->diffuse,  "uDiffuse",  prog);
    gkUniformColor(&specGloss->specular, "uSpecular", prog);

    if (specGloss->diffuseMap)
      gkUniformTex(ctx, mat, specGloss->diffuseMap, "uDiffuse", prog);

    if (specGloss->specGlossMap)
      gkUniformTex(ctx, mat, specGloss->specGlossMap, "uSpecGloss", prog);
  }

  if (techn->ambient) {
    gkUniformColorDesc(ctx, mat, techn->ambient,  "uAmbient",  prog);
    /* TODO: */
//    loc = gkUniformLoc(prog, "uIAmbient");
//    glUniform4fv(loc, 1, (vec4){0.1f, 0.1f, 0.1f, 1.0f});
  }

  if (techn->diffuse)
    gkUniformColorDesc(ctx, mat, techn->diffuse,  "uDiffuse",  prog);

  if (techn->specular)
    gkUniformColorDesc(ctx, mat, techn->specular, "uSpecular", prog);

  if (techn->emission)
    gkUniformColorDesc(ctx, mat, techn->emission, "uEmission", prog);

  if (techn->occlusion && techn->occlusion->tex) {
    gkUniformTex(ctx, mat, techn->occlusion->tex, "uOcclusion", prog);
    gkUniform1f(prog, "uOcclusionStrength", techn->occlusion->strength);
  }

  if (techn->normal && techn->normal->tex) {
    gkUniformTex(ctx, mat, techn->normal->tex, "uNormal", prog);
    gkUniform1f(prog, "uNormalScale", techn->normal->scale);
  }

  if (techn->transparent) {
    GkTransparent *transp;
    
    transp = techn->transparent;
    if (transp->color) {
      gkUniformColorDesc(ctx, mat, transp->color, "uTransparent", prog);
    } else {
      loc = gkUniformLoc(prog, "uTransparent");
      glUniform4fv(loc, 1, (vec4){1.0f, 1.0f, 1.0f, 1.0f});
    }
    
    loc = gkUniformLoc(prog, "uTransparency");
    glUniform1f(loc, transp->amount);

    if (transp->opaque == GK_OPAQUE_MASK)
      gkUniform1f(prog, "uAlphaCutoff", transp->cutoff);
  }
  
  if (techn->reflective) {
    GkReflective *refl;
    refl = techn->reflective;
    
    if (refl->color)
      gkUniformColorDesc(ctx, mat, refl->color, "uReflective", prog);
    
    loc = gkUniformLoc(prog, "uReflectivity");
    glUniform1f(loc, refl->amount);
  }
  
  loc = gkUniformLoc(prog, "uIndexOfRefraction");
  glUniform1f(loc, mat->indexOfRefraction);
  
  prog->lastMaterial  = mat;
  prog->updtMaterials = false;
}
