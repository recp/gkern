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
