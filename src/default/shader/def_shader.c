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

#include "../../common.h"
#include "def_shader.h"

const char*
gk_def_shader_vert(GK_DEF_SHADER shader) {

  switch (shader) {
    case GK_DEF_SHADER_CUBE:
      return
#include "gk_cube.vert"
      ;
      break;
    case GK_DEF_SHADER_DRAWBONE:
      return
#include "gk_drawbone.vs"
      ;
      break;
    default:
      return
#include "gk_default.vert"
      ;
  }
}

const char*
gk_def_shader_frag(GK_DEF_SHADER shader) {
  switch (shader) {
    case GK_DEF_SHADER_CUBE:
      return
#include "gk_cube.frag"
      ;
      break;
    case GK_DEF_SHADER_DRAWBONE:
      return
#include "gk_drawbone.fs"
      ;
      break;
    default:
      return
#include "gk_default.frag"
      ;
  }
}
