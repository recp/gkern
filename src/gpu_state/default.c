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
  
  .texStates = NULL
};

_gk_hide
void
gkSetDefaultState(GkContext * __restrict ctx) {
  if (ctx->currState)
    return;
  
  ctx->currState = malloc(sizeof(gk__defstate));
  memcpy(ctx->currState,
         &gk__defstate,
         sizeof(gk__defstate));
}
