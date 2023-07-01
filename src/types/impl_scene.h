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

#ifndef impl_scene_h
#define impl_scene_h

#include "../../include/gk/scene.h"
#include "../../include/gk/node.h"
#include "../../include/gk/animation.h"
#include "impl_node.h"

#include <ds/forward-list.h>
#include <tm/tm.h>

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
  FListItem         *anims;

  FListItem         *instSkins;
  FListItem         *instMorphs;
  GkPipeline        *clearPipeline;
  GkRenderAfterClearFunc onClear;
  void              * onClearObj;

  GkRenderPathFn     rp;
  GkRenderPathFn     renderFunc;
  GkRenderPathFn     lightIterFunc;
  GkRenderPathType   rpath;
  int32_t            internalFormat;
  size_t             centercount;
  float              backingScale;
  bool               transpPass;
} GkSceneImpl;

#endif /* impl_scene_h */
