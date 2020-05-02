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
