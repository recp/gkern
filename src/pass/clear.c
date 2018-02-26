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
gkClearColor(GkColorOutput *colorOutput) {
  if (colorOutput->clear->color) {
    glClearBufferfv(GL_COLOR,
                    colorOutput->drawIndex,
                   *colorOutput->clear->color);
    return;
  }

  /* TODO: add this to state manager */
  glClearBufferfv(GL_COLOR, colorOutput->drawIndex, GLM_VEC4_ZERO);
}

GK_EXPORT
void
gkClearColorAt(GkOutput *output, int32_t buffIndex) {
  GkColorOutput *color;
  int32_t        i;

  color = output->color;
  for (i = 0; i < buffIndex && color; i++)
    color = color->next;

  if (color && i == buffIndex)
    gkClearColor(color);
}

GK_EXPORT
void
gkClearColors(GkOutput *output) {
  GkColorOutput *color;

  color = output->color;
  while (color) {
    gkClearColor(color);
    color = color->next;
  }
}
