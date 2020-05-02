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

#include "common.h"
#include "../include/gk/bbox.h"
#include "../include/gk/scene.h"

#include <string.h>

void
gkTransformAABB(GkTransform * __restrict trans,
                GkBBox                   bbox[2]) {
  glm_aabb_transform(bbox[0], trans->world, bbox[1]);
}

GK_EXPORT
bool
gkSceneIs2D(GkScene *scene) {
  return glm_eq(scene->bbox[1][0] - scene->bbox[0][0], 0.0f)
         || glm_eq(scene->bbox[1][1] - scene->bbox[0][1], 0.0f)
         || glm_eq(scene->bbox[1][2] - scene->bbox[0][2], 0.0f);
}
