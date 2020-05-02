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
gkActiveTexture(GkContext * __restrict ctx,
                uint32_t               unit) {
  GkGPUStates *ast;
  
  ast = ctx->currState;
  if (ast->activeTex == unit)
    return;
  
  ast->activeTex = unit;
  glActiveTexture(GL_TEXTURE0 + unit);
}

GK_EXPORT
void
gkBindTextureTo(GkContext * __restrict ctx,
                uint32_t               unit,
                uint32_t               target,
                uint32_t               texid) {
  GkTextureState *state;
  GkGPUStates    *ast;

  ast   = ctx->currState;
  state = gkGetOrCreatTexState(ctx, unit, target);

  if (ast->activeTex != unit)
    gkActiveTexture(ctx, unit);

  if (state->texid != texid)
    gkBindTexture(ctx, target, texid);
}

GK_EXPORT
void
gkBindTexture(GkContext * __restrict ctx,
              uint32_t               target,
              uint32_t               texid) {
  GkTextureState *state;
  GkGPUStates    *ast;
  
  ast = ctx->currState;
  
  state = gkGetOrCreatTexState(ctx, ast->activeTex, target);
  if (state->texid == texid)
    return;

  state->texid = texid;
  glBindTexture(target, texid);
}
