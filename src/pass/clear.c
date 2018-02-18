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
                    poutColor->drawIndex,
                   *poutColor->clear->color);
    return;
  }

  /* TODO: add this to state manager */
  glClearBufferfv(GL_COLOR, poutColor->drawIndex, GLM_VEC4_ZERO);
}

GK_EXPORT
void
gkClearColorAt(GkPassOut *pout, int32_t buffIndex) {
  GkPassOutColor *color;
  int32_t         i;

  color = pout->color;
  for (i = 0; i < buffIndex && color; i++)
    color = color->next;

  if (color && i == buffIndex)
    gkClearColor(color);
}

GK_EXPORT
void
gkClearColors(GkPassOut *pout) {
  GkPassOutColor *color;

  color = pout->color;
  while (color) {
    gkClearColor(color);
    color = color->next;
  }
}
