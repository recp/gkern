/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/pass.h"

GK_EXPORT
void
gkClearColor(GkPassOutColor *poutColor) {
  if (poutColor->clear->color) {
    glClearBufferfv(GL_COLOR,
                    poutColor->attachment,
                   *poutColor->clear->color);
    return;
  }

  /* TODO: add this to state manager */
  glClearBufferfv(GL_COLOR, poutColor->attachment, GLM_VEC4_ZERO);
}
