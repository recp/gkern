/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
    assert((uint32_t)pli < GK_ARRAY_LEN(gk_glcontextPLI));
    return gk_glcontextPLI[pli];
  }

  assert((uint32_t)pli < GK_ARRAY_LEN(GK_PLI));

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
