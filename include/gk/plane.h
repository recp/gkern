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

#ifndef plane_h
#define plane_h

#include <cglm/cglm.h>

typedef enum GkPlaneIndex {
  GK_PLANE_LEFT   = 0,
  GK_PLANE_RIGHT  = 1,
  GK_PLANE_BOTTOM = 2,
  GK_PLANE_TOP    = 3,
  GK_PLANE_NEAR   = 4,
  GK_PLANE_FAR    = 5
} GkPlaneIndex;

typedef vec4 GkPlane;

#endif /* plane_h */
