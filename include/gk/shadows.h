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

#ifndef shadows_h
#define shadows_h

#include "common.h"

struct GkScene;
struct GkOutput;

/* Shadow technique type */

typedef enum GkShadowTechnType {
  GK_SHADOW_AUTO               = 0,
  GK_SHADOW_DISABLED           = 1,
  GK_SHADOW_BASIC_SHADOWMAP    = 1,
  GK_SHADOW_CSM                = 2,
  GK_SHADOW_COLORED_STOCHASTIC = 3,
  GK_SHADOW_DEFAULT            = GK_SHADOW_DISABLED
} GkShadowTechnType;

GK_EXPORT
void
gkSetShadowTechn(GkShadowTechnType techn);

GK_EXPORT
GkShadowTechnType
gkShadowTechn(void);

GK_EXPORT
uint32_t
gkShadowSplit(void);

GK_EXPORT
void
gkSetShadowSplit(uint32_t splitCount);

void
gkRenderShadowMapTo(GkScene         * __restrict scene,
                    struct GkOutput * __restrict output);

#endif /* transparency_h */
