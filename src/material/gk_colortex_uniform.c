/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_colortex_uniform.h"
#include "../gk_uniform.h"

#include <string.h>

void
gkUniformColorOrTex(GkColorOrTex * __restrict crtx,
                    char         * __restrict buf,
                    char         * __restrict name,
                    GLuint                    prog) {
  char  uname[64];
  GLint loc;
  GkColorMethod method;

  strcpy(uname, name);
  strcpy(uname + strlen(uname), ".");

  method = crtx->method;

  if (method != GK_DISCARD
      && method != GK_ONLY_USE_COLOR
      && method != GK_ONLY_USE_TEX
      && method != GK_MIX_COLOR_AND_TEX)
    method = GK_DISCARD;

  strcpy(uname + strlen(uname), "method");
  loc = gkGetUniformLoc(prog, buf, uname);
  glUniform1ui(loc, (uint32_t)crtx->method);

  if (method == GK_DISCARD)
    return;

  if (method == GK_ONLY_USE_COLOR
      || method == GK_MIX_COLOR_AND_TEX) {
    strcpy(uname + strlen(uname), "color");
    loc = gkGetUniformLoc(prog, buf, uname);
    glUniform4fv(loc, 1, crtx->color.vec);
  }

  if ((method == GK_ONLY_USE_TEX
      || method == GK_MIX_COLOR_AND_TEX)
      && crtx->tex) {
    GLuint unit;

    unit = 0;
    if (crtx->tex->sampler)
      unit = crtx->tex->sampler->unit;

    strcpy(uname + strlen(uname), "tex");
    loc = gkGetUniformLoc(prog, buf, uname);
    glUniform1i(loc, unit);
  }
}
