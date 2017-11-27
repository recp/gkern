/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
  sceneImpl = calloc(sizeof(*sceneImpl), 1);

  sceneImpl->transfCacheSlots = flist_new(NULL);

  sceneImpl->pub.flags          = GK_SCENEF_INIT | GK_SCENEF_ONCE;
  sceneImpl->pub.backingScale   = 1.0f;
  sceneImpl->pub.internalFormat = GL_RGBA;
  sceneImpl->pub.ctx            = context;

  return &sceneImpl->pub;
}
