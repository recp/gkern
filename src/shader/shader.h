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

#ifndef src_shader_h
#define src_shader_h

#include "../../include/gk/gk.h"
#include "../../include/gk/shader.h"

typedef enum GkShaderFlags {
  GK_SHADER_FLAG_MVP     = 1,
  GK_SHADER_FLAG_MV      = 2,
  GK_SHADER_FLAG_NM      = 3,
  GK_SHADER_FLAG_NMU     = 4,
  GK_SHADER_FLAG_VP      = 5,
  GK_SHADER_FLAG_BUILTIN = GK_SHADER_FLAG_MVP
                         | GK_SHADER_FLAG_MV
                         | GK_SHADER_FLAG_NM
                         | GK_SHADER_FLAG_NMU
} GkShaderFlags;

GkPipeline*
gkGetOrCreatProgByName(const char   *commonName,
                       const char   *shaderSources[],
                       GLenum        shaderTypes[],
                       uint32_t      shaderCount,
                       GkShaderFlags flags);

void
gk_shaders_init(void);

void
gk_shaders_deinit(void);

#endif /* src_shader_h */
