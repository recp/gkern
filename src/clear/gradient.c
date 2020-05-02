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
