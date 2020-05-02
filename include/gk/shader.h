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

#ifndef gk_shader_h
#define gk_shader_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include <cglm/cglm.h>
#include <OpenGL/gl3.h>
#include <stdio.h>

typedef struct GkShader {
  struct GkShader *next;
  GLuint           shaderId;
  GLuint           shaderType;
  GLboolean        isValid;
} GkShader;

GLuint
gkShaderLoadFromFile(GLenum shaderType,
                     const char * __restrict path);

int
gkShaderLoadFromFolder(const char * __restrict path,
                       GkShader ** __restrict dest);

GLuint
gkShaderLoad(GLenum shaderType,
             const char * __restrict source);

GLuint
gkShaderLoadN(GLenum  shaderType,
              char   *source[],
              size_t  count);

void
gkAttachShaders(GLuint program,
                GkShader * __restrict shaders);

void
gkShaderLogInfo(GLuint shaderId,
                FILE * __restrict file);

void
gkUniformMat4(GLint location, mat4 matrix);

GkShader*
gkShaderByName(const char *name);

void
gkShaderSetName(GkShader *shader, const char *name);

#ifdef __cplusplus
}
#endif
#endif /* gk_shader_h */
