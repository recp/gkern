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
