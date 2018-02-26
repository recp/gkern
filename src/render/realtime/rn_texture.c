/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "rn_prim.h"
#include "rn_texture.h"
#include "../../shader/builtin_shader.h"
#include "../../../include/gk/prims/builtin-prim.h"

void
gkRenderTexture(GkScene * __restrict scene, GkPass * __restrict pass) {
  GkProgram *prog;

  glDisable(GL_DEPTH_TEST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  prog = gkBuiltinProg(GK_BUILTIN_PROG_RTT);

  gkUseProgram(gkContextOf(scene), prog);
  gkBindRenderTargetTo(scene, pass, 0, prog, 0, "uColorTex");
  gkRenderBuiltinPrim(scene, GK_PRIM_TEXQUAD);

  glEnable(GL_DEPTH_TEST);
}

void
gkBlit(GkScene  * __restrict scene,
       GkOutput * __restrict src,
       GkOutput * __restrict dst,
       int32_t               colorIndex) {
  float w, h;

  w = scene->vrect.size.w * scene->backingScale;
  h = scene->vrect.size.h * scene->backingScale;

  glBindFramebuffer(GL_READ_FRAMEBUFFER, src->fbo);
  glReadBuffer(GL_COLOR_ATTACHMENT0 + colorIndex);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->fbo);

  glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
