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

#include "common.h"

GK_EXPORT
void
gkEnableDepthTest(GkContext * __restrict ctx) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (state->test)
    return;

  state->test = true;

  glEnable(GL_DEPTH_TEST);
}

GK_EXPORT
void
gkDisableDepthTest(GkContext * __restrict ctx) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (!state->test)
    return;

  state->test = false;

  glDisable(GL_DEPTH_TEST);
}

GK_EXPORT
void
gkDepthMask(GkContext * __restrict ctx, GLboolean flag) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (state->mask == flag)
    return;

  state->mask = flag;

  glDepthMask(flag);
}

GK_EXPORT
void
gkDepthFunc(GkContext * __restrict ctx, GLenum func) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (state->func == func)
    return;

  state->func = func;

  glDepthFunc(func);
}
