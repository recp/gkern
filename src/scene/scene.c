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
#include "../../include/gk/gk.h"
#include "../../include/gk/scene.h"
#include "../types/impl_scene.h"

#include <ds/forward-list.h>
#include <string.h>

GK_EXPORT
GkScene*
gkAllocScene(GkContext * __restrict context) {
  GkSceneImpl *sceneImpl;
  sceneImpl = calloc(1, sizeof(*sceneImpl));

  sceneImpl->transfCacheSlots = flist_new(NULL);

  sceneImpl->pub.flags          = GK_SCENEF_INIT | GK_SCENEF_ONCE;
  sceneImpl->pub.backingScale   = 1.0f;
  sceneImpl->pub.internalFormat = GL_RGBA;
  sceneImpl->pub.ctx            = context;

  return &sceneImpl->pub;
}

GK_EXPORT
void
gkSceneSetOnClear(GkScene                * __restrict scene,
                  GkRenderAfterClearFunc onClear,
                  void                   * __restrict obj) {
  GkSceneImpl *sceneImpl;

  sceneImpl             = (GkSceneImpl *)scene;
  sceneImpl->onClear    = onClear;
  sceneImpl->onClearObj = obj;
}
