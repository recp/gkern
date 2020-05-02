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

#include "../common.h"

#include "shadows.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/scene.h"
#include "../../include/gk/render.h"

#include "../../include/gk/prims/builtin-prim.h"
#include "../shader/builtin_shader.h"
#include "../program/uniform_cache.h"

#include "../shader/shader.h"
#include "../state/gpu.h"

#include "builtin/basic.h"
#include "builtin/csm.h"
#include <ds/forward-list.h>

void *GK_SHADOWS_HANDLE                = &GK_SHADOWS_HANDLE;
GkShadowTechnType   gk__shadow_techn   = GK_SHADOW_CSM;
gkSetupShadowsFunc  gk__setupShadowsFn = gkSetupShadowMapCSM;
gkRenderShadowsFunc gk__rnShadowsFn    = gkRenderShadowMapCSM;
uint32_t            gk__shadSplitCount = 4;

mat4 gk__biasMatrix = {
  0.5f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.5f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f
};

GK_EXPORT
void
gkSetShadowTechn(GkShadowTechnType techn) {
  switch (techn) {
    case GK_SHADOW_BASIC_SHADOWMAP:
      gk__setupShadowsFn = gkSetupBasicShadowMap;
      gk__rnShadowsFn    = gkRenderBasicShadowMap;
      break;
    case GK_SHADOW_CSM:
      gk__setupShadowsFn = gkSetupShadowMapCSM;
      gk__rnShadowsFn    = gkRenderShadowMapCSM;
      break;
    default:
      break;
  }
}

GK_EXPORT
GkShadowTechnType
gkShadowTechn(void) {
  return gk__shadow_techn;
}

GK_EXPORT
void
gkSetShadowSplit(uint32_t splitCount) {
  gk__shadSplitCount = splitCount;
}

GK_EXPORT
uint32_t
gkShadowSplit() {
  return gk__shadSplitCount;
}

GkShadowMap*
gkSetupShadows(GkScene * __restrict scene,
               GkLight * __restrict light) {
  return gk__setupShadowsFn(scene, light);
}

void
gkRenderShadows(GkScene * __restrict scene,
                GkLight * __restrict light) {
  gk__rnShadowsFn(scene, light);
}

GK_EXPORT
void
gkEnableShadows(GkScene * __restrict scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;

  if (!sceneImpl->shadows)
    sceneImpl->shadows = hash_new_i32(8);

  sceneImpl->rpath = GK_RNPATH_SCENE_PERLIGHT;
  sceneImpl->rp    = gkScenePerLightRenderPath;
  scene->flags    |= GK_SCENEF_SHADOWS;

  flist_insert(gkContextOf(scene)->samplers, GK_SHADOWS_HANDLE);
}

GK_EXPORT
void
gkDisableShadows(GkScene * __restrict scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;

  if (scene->flags & GK_SCENEF_TRANSP) {
    sceneImpl->rpath = GK_RNPATH_SCENE_PERLIGHT;
    sceneImpl->rp    = gkScenePerLightRenderPath;
  } else {
    sceneImpl->rpath = GK_RNPATH_MODEL_PERLIGHT;
    sceneImpl->rp    = gkModelPerLightRenderPath;
  }

  scene->flags &= ~GK_SCENEF_SHADOWS;

  flist_remove_by(gkContextOf(scene)->samplers, GK_SHADOWS_HANDLE);
}
