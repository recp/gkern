/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
gkAllocOutput(void) {
  GkOutput *output;

  output = calloc(1, sizeof(*output));
  glGenFramebuffers(1, &output->fbo);

  return output;
}

GK_EXPORT
GkOutput*
gkCurrentOutput(GkContext * __restrict ctx) {
  GkOutput *output;

  if ((output = ctx->currState->output->renderOutput))
    return output;

  return gkDefaultRenderOut();
}

GK_EXPORT
void
gkBindOutput(GkScene  *scene,
              GkOutput *output) {
  GkOutputState *state;
  GkContext     *ctx;

  ctx   = gkContextOf(scene);
  state = gkGetOrCreatState(ctx, GK_GPUSTATE_OUTPUT);
  if (ctx->currState->output->renderOutput == output)
    return;

  state->renderOutput = output;
  glBindFramebuffer(GL_FRAMEBUFFER, output->fbo);
}

GK_EXPORT
void
gkBindDefaultOutput(GkScene *scene) {
  GkOutputState *state;
  GkContext        *ctx;

  ctx   = gkContextOf(scene);
  state = gkGetOrCreatState(ctx, GK_GPUSTATE_OUTPUT);
  if (ctx->currState->output->renderOutput == NULL)
    return;

  state->renderOutput = NULL;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
