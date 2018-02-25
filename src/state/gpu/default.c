/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "default.h"
#include "common.h"
#include <string.h>

#define GK__STATE_BASE(T) \
   .base = { \
      .prev    = NULL, \
      .next    = NULL, \
      .type    = T, \
      .index   = 0, \
      .indexed = false \
   }

GkGPUStates gk__defstate = {
  .depthState = &(GkDepthState){
    GK__STATE_BASE(GK_GPUSTATE_DEPTH),
    .test = true,
    .func = GL_LESS,
    .mask = GL_TRUE,
  },

  .faceState = &(GkFaceState){
    GK__STATE_BASE(GK_GPUSTATE_CULLFACE),
    .cull      = true,
    .face      = GL_BACK,
    .frontFace = GL_CCW
  },

  .blendState = &(GkBlendState){
    GK__STATE_BASE(GK_GPUSTATE_BLEND),
    .blend = false,
    .eq    = GL_FUNC_ADD,
    .src   = GL_ONE,
    .dst   = GL_ZERO
  },

  .outputState = &(GkOutputState){
    GK__STATE_BASE(GK_GPUSTATE_OUTPUT),
    .renderOutput = NULL
  },

  .activeTex    = 0,
  .texStates    = NULL,
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
}
