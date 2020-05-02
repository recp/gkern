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
gkEnableBlend(GkContext * __restrict ctx) {
  GkBlendState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_BLEND);
  if (state->blend)
    return;

  state->blend = true;

  glEnable(GL_BLEND);
}

GK_EXPORT
void
gkDisableBlend(GkContext * __restrict ctx) {
  GkBlendState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_BLEND);
  if (!state->blend)
    return;

  state->blend = false;

  glDisable(GL_BLEND);
}

GK_EXPORT
void
gkBlendEq(GkContext * __restrict ctx, GLenum eq) {
  GkBlendState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_BLEND);
  if (state->eq == eq)
    return;

  state->eq = eq;

  glBlendEquation(eq);
}

GK_EXPORT
void
gkBlendFunc(GkContext * __restrict ctx, GLenum src, GLenum dst) {
  GkBlendState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_BLEND);

  state->src = src;
  state->dst = dst;

  glBlendFunc(src, dst);
}

GK_EXPORT
void
gkEnableBlendi(GkContext * __restrict ctx, GLuint buf) {
  GkBlendState *state;

  state = gkGetOrCreatStatei(ctx, buf, GK_GPUSTATE_BLEND);
  if (state->blend)
    return;

  state->blend = true;

  glEnablei(GL_BLEND, buf);
}

GK_EXPORT
void
gkDisableBlendi(GkContext * __restrict ctx, GLuint buf) {
  GkBlendState *state;

  state = gkGetOrCreatStatei(ctx, buf, GK_GPUSTATE_BLEND);
  if (!state->blend)
    return;

  state->blend = false;

  glDisablei(GL_BLEND, buf);
}

GK_EXPORT
void
gkBlendFunci(GkContext * __restrict ctx,
             GLint                  buf,
             GLenum                 src,
             GLenum                 dst) {
  GkBlendState *state;

  state = gkGetOrCreatStatei(ctx, buf, GK_GPUSTATE_BLEND);

  state->src = src;
  state->dst = dst;

  glBlendFunci(buf, src, dst);
}
