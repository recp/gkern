/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"

GK_EXPORT
void
gkSrcBuff(GkContext * __restrict ctx, GLenum buff) {
  GkFramebuffState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_BLEND);
  if (state->readbuff == buff)
    return;

  state->readbuff = buff;

  glReadBuffer(buff);
}

GK_EXPORT
void
gkDstBuff(GkContext * __restrict ctx, GLenum buff) {
  GkFramebuffState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_BLEND);
  if (state->drawbuff == buff)
    return;

  state->drawbuff = buff;

  glDrawBuffer(buff);
}
