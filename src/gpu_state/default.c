/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "default.h"
#include "common.h"
#include <string.h>

GkGPUStates gk__defstate = {
  .depthState = {
    .depthTest = true,
    .depthFunc = GL_LESS
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
  
  ctx->currState = calloc(sizeof(gk__defstate), 1);
  memcpy(ctx->currState,
         &gk__defstate,
         sizeof(gk__defstate));
}
