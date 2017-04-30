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

  strcpy(uname, name);
  strcpy(uname + strlen(uname), ".");

  strcpy(uname + strlen(uname), "color");
  loc = gkGetUniformLoc(prog, buf, uname);
  glUniform4fv(loc, 1, crtx->color.vec);
}
