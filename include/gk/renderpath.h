/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef render_path_h
#define render_path_h

#include "common.h"

struct GkScene;

typedef void (*GkRenderPathFn) (struct GkScene * __restrict scene);

typedef enum GkRenderPathType {
  GK_RNPATH_MODEL_PERLIGHT = 1,
  GK_RNPATH_SCENE_PERLIGHT = 2  /* for shadowmaps */
} GkRenderPathType;

GK_EXPORT
void
gkSetRenderPath(struct GkScene * __restrict scene,
                GkRenderPathType            rpath);

#endif /* render_path_h */
