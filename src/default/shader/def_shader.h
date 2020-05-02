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

#ifndef gk_def_shader_h
#define gk_def_shader_h

#include "../../../include/gk/gk.h"

typedef enum GK_DEF_SHADER {
  GK_DEF_SHADER_DEFAULT  = 0,
  GK_DEF_SHADER_CUBE     = 1,
  GK_DEF_SHADER_DRAWBONE = 2
} GK_DEF_SHADER;

const char*
gk_def_shader_vert(GK_DEF_SHADER shader);

const char*
gk_def_shader_frag(GK_DEF_SHADER shader);

#endif /* gk_def_shader_h */
