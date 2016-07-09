/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_shader_h
#define gk_shader_h

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <mk.h>

typedef struct GkShader {
  struct GkShader *next;

  GLuint    shaderId;
  GLuint    shaderType;
  GLboolean isValid;
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

void
gkAttachShaders(GLuint program,
                GkShader * __restrict shaders);

void
gkShaderLogInfo(GLuint shaderId,
                FILE * __restrict file);

void
gkUniformMatrix(GLint location, MkMatrix *matrix);

void
gkUniformMatrix4f(GLint location, MkMatrix *matrix);

#endif /* gk_shader_h */
