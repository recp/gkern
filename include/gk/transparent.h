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

#ifndef transparency_h
#define transparency_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

struct GkScene;

/* Transparent technique type */

typedef enum GkTranspTechnType {
  GK_TRANSP_AUTO             = 0,
  GK_TRANSP_DISABLED         = 1,
  GK_TRANSP_ALPHA_BLENDED    = 2, /* ordering is required */
  GK_TRANSP_WEIGHTED_BLENDED = 3,
  GK_TRANSP_PHENOMENOLOGICAL = 4,

  GK_TRANSP_DEFAULT          = GK_TRANSP_WEIGHTED_BLENDED
} GkTranspTechnType;

GK_EXPORT
void
gkSetTranspTechn(GkTranspTechnType techn);

GK_EXPORT
GkTranspTechnType
gkTranspTechn(void);

GK_EXPORT
void
gkEnableTransp(GkScene * __restrict scene);

GK_EXPORT
void
gkDisableTransp(GkScene * __restrict scene);

#ifdef __cplusplus
}
#endif
#endif /* transparency_h */
