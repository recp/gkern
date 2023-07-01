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

#ifndef gk_program_h
#define gk_program_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "shader.h"
#include "uniform.h"

#include <ds/rb.h>
#include <ds/hash.h>
#include <ds/forward-list-sep.h>
  
#include <OpenGL/gl3.h>
#include <stdio.h>
#include <stdbool.h>

struct GkMaterial;
struct GkLight;
struct GkShader;
struct GkScene;
struct GkContext;

typedef struct GkPipeline {
  FListItem         *vertex;
  HTable            *uniforms;
  struct GkShader   *shaders;
  struct GkMaterial *lastMaterial;
  struct GkLight    *lastLight;
  uint32_t           refc;
  GLint              progId;
  GLint              mvpi;
  GLint              mi;
  GLint              mvi;
  GLint              vpi;
  GLint              nmi;
  GLint              nmui;
  GLint              vi;
  GLint              pi;
  GLint              inv_vi;
  GLint              inv_pi;
  GLint              inv_vpi;
  GLint              neari;
  GLint              fari;
  bool               updtLights;
  bool               updtMaterials;
} GkPipeline;

void
gkProgramLogInfo(GLuint progId, FILE * __restrict file);

bool
gkProgramIsValid(GLuint progId);

GkPipeline*
gkAllocPipeline(void);

GkPipeline*
gkNewPipeline(GkShader *shaders,
              void    (*beforeLink)(GkPipeline *prog, void *data),
              void     *userData);

GkPipeline*
gkDefaultProgram(void);

GLint
gkCurrentProgram(void);

GK_EXPORT
void
gkUseProgram(struct GkContext *ctx,
             GkPipeline       *prog);

GkPipeline*
gkGetPipeline(char       *name,
              GkPipeline *(creatCb)(char *name, void *userData),
              void       *userData);

#ifdef __cplusplus
}
#endif
#endif /* gk_program_h */
