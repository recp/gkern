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
#include "prim.h"
#include "texture.h"
#include "../../shader/builtin_shader.h"
#include "../../state/gpu.h"
#include "../../../include/gk/prims/builtin-prim.h"

void
gkRenderTexture(GkScene * __restrict scene, GkPass * __restrict pass) {
  GkPipeline *prog;
  GkContext *ctx;

  ctx  = gkContextOf(scene);
  prog = gkBuiltinProg(GK_BUILTIN_PROG_RTT);

  gkPushState(ctx);
  gkDisableDepthTest(ctx);

  gkUseProgram(gkContextOf(scene), prog);
  gkBindRenderTargetTo(scene, pass, 0, prog, 0, "uColorTex");
  gkRenderBuiltinPrim(scene, GK_PRIM_TEXQUAD);

  gkPopState(ctx);
}

void
gkBlit(GkScene  * __restrict scene,
       GkOutput * __restrict src,
       GkOutput * __restrict dst,
       int32_t               colorIndex) {
  float w, h;

  w = scene->viewport[2] * scene->backingScale;
  h = scene->viewport[3] * scene->backingScale;

  glBindFramebuffer(GL_READ_FRAMEBUFFER, src->fbo);
  glReadBuffer(GL_COLOR_ATTACHMENT0 + colorIndex);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->fbo);

  glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
