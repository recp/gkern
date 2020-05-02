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

#ifndef rn_texture_h
#define rn_texture_h

#include "../../../include/gk/gk.h"
#include "../../../include/gk/pass.h"

void
gkRenderTexture(GkScene * __restrict scene, GkPass * __restrict pass);

void
gkBlit(GkScene  * __restrict scene,
       GkOutput * __restrict src,
       GkOutput * __restrict dst,
       int32_t               colorIndex);

#endif /* rn_texture_h */
