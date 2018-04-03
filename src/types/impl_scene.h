/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef impl_scene_h
#define impl_scene_h

#include "../../include/gk/scene.h"
#include "../../include/gk/node.h"
#include "impl_node.h"

#include <ds/forward-list.h>

typedef struct GkSceneImpl {
  GkScene            pub;
  struct FListItem  *transpPrims;
  struct GkLight    *forLight;
  void              *shadows;
  void              *transp;
  struct GkPass     *overridePass;     /* override all passes    */
  struct GkMaterial *overrideMaterial; /* override all materials */
  FList             *transfCacheSlots;

  GkNodePage        *nodePages;
  GkNodePage        *lastPage;

  GkRenderPathFn     rp;
  GkRenderPathFn     renderFunc;
  GkRenderPathFn     lightIterFunc;
  GkRenderPathType   rpath;
  int32_t            internalFormat;
  float              backingScale;
  bool               transpPass;
} GkSceneImpl;

#endif /* impl_scene_h */
