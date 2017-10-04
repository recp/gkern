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
    loc = gkGetUniformLoc(prog, buf, name);
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

    loc = gkGetUniformLoc(prog, buf, name);
    glUniform1i(loc, unit);
  }
}
