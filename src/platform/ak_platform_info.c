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
#include "../../include/gk/gk.h"
#include "../../include/gk/platform.h"

/* Default expected values */
GLint GK_PLI[] =
{
  16                               /* 0:  _MAX_TEX_UNIT                */
};

void  *gk_glcontext    = NULL;
GLint *gk_glcontextPLI = NULL;

void
gk_pl_fetchPLI(void);

GLint
gkPlatfomInfo(GkPlatformInfo pli) {
  if (gk_glcontext) {
#ifdef DEBUG
    assert((uint32_t)pli < GK_ARRAY_LEN(gk_glcontextPLI));
#endif
    return gk_glcontextPLI[pli];
  }

#ifdef DEBUG
  assert((uint32_t)pli < GK_ARRAY_LEN(GK_PLI));
#endif
  return GK_PLI[pli];
}

void
gk_pl_fetchPLI() {
  if (!gk_glcontext)
    return;

  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &gk_glcontextPLI[0]);
}

void
gkSetGLContext(void *glcontext) {
  if (gk_glcontext) {
    free(gk_glcontextPLI);

    gk_glcontextPLI = NULL;
    gk_glcontext    = NULL;
  }

  if (glcontext) {
    gk_glcontext    = glcontext;
    gk_glcontextPLI = calloc(GK_ARRAY_LEN(GK_PLI), sizeof(GLint));
    gk_pl_fetchPLI();
  }
}
