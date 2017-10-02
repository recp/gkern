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
  char          uname[64];
  GLint         loc;
  GkColorMethod method;
  size_t        startOff;

  strcpy(uname, name);
  strcpy(uname + strlen(uname), ".");
  startOff = strlen(uname);

  method = crtx->method;

  if (method != GK_COLOR_DISCARD
      && method != GK_COLOR_COLOR
      && method != GK_COLOR_TEX)
    method = GK_COLOR_DISCARD;

  strcpy(uname + startOff, "method");
  loc = gkGetUniformLoc(prog, buf, uname);
  glUniform1ui(loc, (uint32_t)crtx->method);

  if (method == GK_COLOR_DISCARD || !crtx->val)
    return;

  if (method == GK_COLOR_COLOR) {
    GkColor *color;

    color = crtx->val;
    strcpy(uname + startOff, "color");
    loc = gkGetUniformLoc(prog, buf, uname);
    glUniform4fv(loc, 1, color->vec);
  } else if (method == GK_COLOR_TEX) {
    GkTexture *tex;
    GLuint     unit;

    tex  = crtx->val;
    unit = 0;
    if (tex->sampler) {
      unit = tex->sampler->unit;
      glActiveTexture(GL_TEXTURE0 + unit);
      glBindTexture(tex->target, tex->index);
    }

    strcpy(uname + startOff, "tex");
    loc = gkGetUniformLoc(prog, buf, uname);
    glUniform1i(loc, unit);
  }
}
