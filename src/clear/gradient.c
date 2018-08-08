/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/clear.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/gpu_state.h"
#include "../../include/gk/prims/builtin-prim.h"
#include "../types/impl_scene.h"
#include "../shader/builtin_shader.h"

GK_EXPORT
void
gkClearEffect(GkScene *scene, GkPipeline *pip) {
  GkContext *ctx;
  vec2       size;

  ctx     = gkContextOf(scene);
  size[0] = scene->viewport[2] * scene->backingScale;
  size[1] = scene->viewport[3] * scene->backingScale;

  gkPushState(ctx);
  gkUseProgram(ctx, pip);
  gkUniform2f(pip, "uSize", size);
  gkRenderBuiltinPrim(scene, GK_PRIM_TEXQUAD);
  gkPopState(ctx);
}

GK_EXPORT
void
gkSetClearEffect(GkScene *scene, GkClearEffect effect) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;

  sceneImpl->clearPipeline = gkBuiltinProg(GK_BUILTIN_PROG_CLR_GRAD_CIRC);
}
