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
#include "../program/uniform_cache.h"
#include "../state/gpu.h"

#include <string.h>

void
gkUniformColor(GkColor   * __restrict color,
               char      * __restrict name,
               GkPipeline * __restrict prog) {
  glUniform4fv(gkUniformLoc(prog, name), 1, color->vec);
}

void
gkUniformColorBuff(GkColor   * __restrict color,
                   char      * __restrict buf,
                   char      * __restrict name,
                   GkPipeline * __restrict prog) {
  glUniform4fv(gkUniformLocBuff(prog, name, buf), 1, color->vec);
}

void
gkUniformTex(GkContext  * __restrict ctx,
             GkMaterial * __restrict mat,
             GkTexture  * __restrict tex,
             char       * __restrict name,
             GkPipeline  * __restrict prog) {
  char        uniformNameBuff[32];
  const char *uniformName;

  if (!tex->sampler)
    return;

  gkBindTextureTo(ctx,
                  ctx->availTexUnit,
                  tex->target,
                  tex->index);

  tex->boundUnit = ctx->availTexUnit;
  flist_sp_insert(&mat->boundTextures, tex);

  if (!(uniformName = tex->sampler->uniformName)) {
    sprintf(uniformNameBuff, "%sTex", name);
    uniformName = uniformNameBuff;
  }

  gkUniform1i(prog, uniformName, tex->boundUnit);
  ctx->availTexUnit++;
}

void
gkUniformColorDesc(GkContext    * __restrict ctx,
                   GkMaterial   * __restrict mat,
                   GkColorDesc * __restrict crtx,
                   char         * __restrict name,
                   GkPipeline    * __restrict prog) {
  GkColorMethod method;

  method = crtx->method;

  if (method != GK_COLOR_DISCARD
      && method != GK_COLOR_COLOR
      && method != GK_COLOR_TEX)
    method = GK_COLOR_DISCARD;

  if (method == GK_COLOR_DISCARD || !crtx->val)
    return;

  if (method == GK_COLOR_COLOR) {
    gkUniformColor(crtx->val, name, prog);
  } else if (method == GK_COLOR_TEX) {
    gkUniformTex(ctx, mat, crtx->val, name, prog);
  }
}

void
gkUniformColorDescBuff(GkContext   * __restrict ctx,
                       GkMaterial  * __restrict mat,
                       GkColorDesc * __restrict crtx,
                       char        * __restrict buf,
                       char        * __restrict name,
                       GkPipeline   * __restrict prog) {
  GkColorMethod method;
  
  method = crtx->method;
  
  if (method != GK_COLOR_DISCARD
      && method != GK_COLOR_COLOR
      && method != GK_COLOR_TEX)
  method = GK_COLOR_DISCARD;
  
  if (method == GK_COLOR_DISCARD || !crtx->val)
  return;
  
  if (method == GK_COLOR_COLOR) {
    gkUniformColorBuff(crtx->val, buf, name, prog);
  } else if (method == GK_COLOR_TEX) {
    GkTexture *tex;
    
    tex = crtx->val;
    if (tex->sampler) {
      const char *uniformName;
      
      gkBindTextureTo(ctx,
                      ctx->availTexUnit,
                      tex->target,
                      tex->index);

      tex->boundUnit = ctx->availTexUnit;
      flist_sp_insert(&mat->boundTextures, tex);
      
      if (!(uniformName = tex->sampler->uniformName))
        uniformName = name;

      glUniform1i(gkUniformLocBuff(prog,
                                   (char *)uniformName,
                                   buf),
                  ctx->availTexUnit);
      ctx->availTexUnit++;
    }
  }
}
