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

#include "../../common.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/pass.h"
#include "../../../include/gk/scene.h"
#include "../../state/gpu.h"

GkOutput gk_def_renderout = {
  .fbo        = 0,
  .depth      = 0,
  .stencil    = 0,
  .color      = NULL,
  .colorCount = 0
};

GK_EXPORT
GkOutput*
gkDefaultRenderOut(void) {
  return &gk_def_renderout;
}

GK_EXPORT
GkOutput*
gkAllocOutput(GkContext * __restrict ctx) {
  GkOutput *output;

  output = calloc(1, sizeof(*output));
  glGenFramebuffers(1, &output->fbo);

  /*
  gkPushState(ctx);
  gkBindOutputFor(ctx, output);
  */

  /* output options, flags here */

  /*
  gkPopState(ctx);
  */

  return output;
}

GK_EXPORT
GkOutput*
gkCurrentOutput(GkContext * __restrict ctx) {
  GkOutput *output;

  if ((output = ctx->currState->output->output))
    return output;

  return gkDefaultRenderOut();
}

GK_EXPORT
void
gkBindOutputFor(GkContext * __restrict ctx, GkOutput *output) {
  GkOutputState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_OUTPUT);
  if (ctx->currState->output->output == output)
    return;

  state->output = output;
  glBindFramebuffer(GL_FRAMEBUFFER, output->fbo);
}

GK_EXPORT
void
gkBindOutput(GkScene *scene, GkOutput *output) {
  gkBindOutputFor(gkContextOf(scene), output);
}

GK_EXPORT
void
gkBindDefaultOutput(GkScene *scene) {
  GkOutputState *state;
  GkContext        *ctx;

  ctx   = gkContextOf(scene);
  state = gkGetOrCreatState(ctx, GK_GPUSTATE_OUTPUT);
  if (ctx->currState->output->output == NULL)
    return;

  state->output = NULL;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
