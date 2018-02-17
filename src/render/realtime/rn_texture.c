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
