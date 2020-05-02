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
gkEnableCullFace(GkContext * __restrict ctx) {
  GkFaceState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_CULLFACE);
  if (state->cull)
    return;

  state->cull = true;

  glEnable(GL_CULL_FACE);
}

GK_EXPORT
void
gkDisableCullFace(GkContext * __restrict ctx) {
  GkFaceState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_CULLFACE);
  if (!state->cull)
    return;

  state->cull = false;

  glDisable(GL_CULL_FACE);
}

GK_EXPORT
void
gkToggleDoubleSided(GkContext * __restrict ctx, bool doubleSided) {
  if (!doubleSided) {
    gkEnableCullFace(ctx);
    return;
  }

  gkDisableCullFace(ctx);
}

GK_EXPORT
void
gkCullFace(GkContext * __restrict ctx, GLenum face) {
  GkFaceState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_CULLFACE);

  state->face = face;

  glCullFace(face);
}

GK_EXPORT
void
gkFrontFace(GkContext * __restrict ctx, GLenum face) {
  GkFaceState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_CULLFACE);

  state->frontFace = face;

  glFrontFace(face);
}
