/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
