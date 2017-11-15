/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef src_shader_h
#define src_shader_h

#include "../../include/gk/gk.h"
#include "../../include/gk/shader.h"

typedef enum GkShaderFlags {
  GK_SHADER_FLAG_MVP = 1,
  GK_SHADER_FLAG_MV  = 2,
  GK_SHADER_FLAG_NM  = 3,
  GK_SHADER_FLAG_NMU = 4
} GkShaderFlags;

GkProgram*
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
