/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "colortex_uniform.h"
#include "../program/uniform_cache.h"

#include <string.h>

void
gkUniformColorOrTex(GkColorOrTex * __restrict crtx,
                    char         * __restrict name,
                    GkProgram    * __restrict prog,
                    uint32_t     * __restrict texUnit) {
  GLint         loc;
  GkColorMethod method;

  method = crtx->method;

  if (method != GK_COLOR_DISCARD
      && method != GK_COLOR_COLOR
      && method != GK_COLOR_TEX)
    method = GK_COLOR_DISCARD;

  if (method == GK_COLOR_DISCARD || !crtx->val)
    return;

  if (method == GK_COLOR_COLOR) {
    GkColor *color;

    color = crtx->val;
    loc   = gkUniformLoc(prog, name);
    glUniform4fv(loc, 1, color->vec);
  } else if (method == GK_COLOR_TEX) {
    GkTexture *tex;

    tex = crtx->val;
    if (tex->sampler) {
      const char *uniformName;

      glActiveTexture(GL_TEXTURE0 + *texUnit);
      glBindTexture(tex->target, tex->index);
      
      if (!(uniformName = tex->sampler->uniformName)) {
        char uniformNameBuff[32];
        sprintf(uniformNameBuff, "%sTex", name);
        uniformName = uniformNameBuff;
      }

      loc = gkUniformLoc(prog, (char *)uniformName);
      glUniform1i(loc, *texUnit);
      (*texUnit)++;
    }
  }
}

void
gkUniformColorOrTexBuff(GkColorOrTex * __restrict crtx,
                        char         * __restrict buf,
                        char         * __restrict name,
                        GkProgram    * __restrict prog,
                        uint32_t     * __restrict texUnit) {
  GLint         loc;
  GkColorMethod method;
  
  method = crtx->method;
  
  if (method != GK_COLOR_DISCARD
      && method != GK_COLOR_COLOR
      && method != GK_COLOR_TEX)
  method = GK_COLOR_DISCARD;
  
  if (method == GK_COLOR_DISCARD || !crtx->val)
  return;
  
  if (method == GK_COLOR_COLOR) {
    GkColor *color;
    
    color = crtx->val;
    loc = gkUniformLocBuff(prog, name, buf);
    glUniform4fv(loc, 1, color->vec);
  } else if (method == GK_COLOR_TEX) {
    GkTexture *tex;
    
    tex = crtx->val;
    if (tex->sampler) {
      const char *uniformName;
      
      glActiveTexture(GL_TEXTURE0 + *texUnit);
      glBindTexture(tex->target, tex->index);
      
      if (!(uniformName = tex->sampler->uniformName))
        uniformName = name;
      
      loc = gkUniformLocBuff(prog, (char *)uniformName, buf);
      glUniform1i(loc, *texUnit);
      (*texUnit)++;
    }
  }
}
