/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "default.h"
#include "common.h"
#include <string.h>

GkGPUStates gk__defstate = {
  .depthState = {
    .depthTest = false,
    .depthFunc = GL_LESS
  },

  .faceState = {
    .cull      = true,
    .face      = GL_BACK,
    .frontFace = GL_CCW
  },

  .blendState = {
    .blend   = GL_FUNC_ADD,
    .sfactor = GL_ONE,
    .dfactor = GL_ZERO
  },

  .activeTex    = 0,
  .texStates    = NULL,
  .outputState  = NULL,
  .prog         = NULL
};

_gk_hide
void
gkSetDefaultState(GkContext * __restrict ctx) {
  if (ctx->currState)
    return;

  ctx->currState = calloc(1, sizeof(gk__defstate));
  memcpy(ctx->currState,
         &gk__defstate,
         sizeof(gk__defstate));

  gkEnableDepthTest(ctx);
  gkEnableCullFace(ctx);
}
