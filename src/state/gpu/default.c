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

GkTextureState gk__deftexst = {
  GK__STATE_BASE(GK_GPUSTATE_TEXTURE),
  .texunit = 0,
  .target  = GL_TEXTURE_2D,
  .texid   = 0
};

GkGPUStates gk__defstate = {
  .depth = &(GkDepthState){
    GK__STATE_BASE(GK_GPUSTATE_DEPTH),
    .test = true,
    .func = GL_LESS,
    .mask = GL_TRUE,
  },

  .face = &(GkFaceState){
    GK__STATE_BASE(GK_GPUSTATE_CULLFACE),
    .cull      = true,
    .face      = GL_BACK,
    .frontFace = GL_CCW
  },

  .blend = &(GkBlendState){
    GK__STATE_BASE(GK_GPUSTATE_BLEND),
    .blend = false,
    .eq    = GL_FUNC_ADD,
    .src   = GL_ONE,
    .dst   = GL_ZERO
  },

  .output = &(GkOutputState){
    GK__STATE_BASE(GK_GPUSTATE_OUTPUT),
    .renderOutput = NULL
  },

  .tex       = NULL,
  .activeTex = 0,
  .prog      = NULL
};

_gk_hide
void
gkInitStates(GkContext * __restrict ctx) {
  if (ctx->currState)
    return;

  ctx->currState = calloc(1, sizeof(*ctx->currState));

  memcpy(ctx->currState, &gk__defstate, sizeof(gk__defstate));
  ctx->currState->tex = hash_new(NULL,
                                 ds_hashfn_ui32p,
                                 ds_cmp_ui32p, 8);
}
