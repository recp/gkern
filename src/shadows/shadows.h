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

#ifndef src_shadows_h
#define src_shadows_h

#include "../../include/gk/gk.h"
#include "../../include/gk/shadows.h"
#include "../../include/gk/pass.h"

typedef struct GkShadowMap {
  GkPass  *pass;
  GkLight *currLight;
  float   *distances;
  mat4    *viewProj;
  int      splitc;
  float    near;
  float    far;
  GkSize   size;
} GkShadowMap;

typedef GkShadowMap* (*gkSetupShadowsFunc)(GkScene * __restrict scene,
                                           GkLight * __restrict light);
typedef void (*gkRenderShadowsFunc)(GkScene * __restrict scene,
                                    GkLight * __restrict light);

GkShadowMap*
gkSetupShadows(GkScene * __restrict scene,
               GkLight * __restrict light);

void
gkRenderShadows(GkScene * __restrict scene,
                GkLight * __restrict light);

#endif /* src_shadows_h */
