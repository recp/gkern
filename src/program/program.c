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

#include "../common.h"
#include "../../include/gk/program.h"
#include "../../include/gk/shader.h"
#include "../default/shader/def_shader.h"

#include "program.h"
#include "../state/gpu.h"
#include <stdlib.h>
#include <string.h>

#include <ds/rb.h>

static RBTree *gk_progs;

void
gkProgramLogInfo(GLuint progId,
                 FILE * __restrict file) {
  char * infoLog;
  GLint  infoLogLen;

  glGetProgramiv(progId,
                 GL_INFO_LOG_LENGTH,
                 &infoLogLen);
  infoLog = malloc(sizeof(*infoLog) * infoLogLen + 1);

  glGetShaderInfoLog(progId,
                     infoLogLen,
                     NULL,
                     infoLog);

  fprintf(file,
          " -- Info Log For Program: %d -- \n",
          progId);

  fwrite(infoLog,
         1,
         infoLogLen,
         file);

  fprintf(file, " --------------------- \n");

  free(infoLog);
}

bool
gkProgramIsValid(GLuint progId) {
  GLint linkStatus;
  glGetProgramiv(progId,
                 GL_LINK_STATUS,
                 &linkStatus);

  return linkStatus == GL_TRUE;
}

GkPipeline*
gkNewPipeline(GkShader *shaders,
              void (*beforeLink)(GkPipeline *prog, void *data),
              void *userData) {
  static GkPipeline *prog;
  GLuint progId;

  prog = calloc(1, sizeof(*prog));
  prog->progId = progId = glCreateProgram();

  gkAttachShaders(progId, shaders);

  if (beforeLink)
    beforeLink(prog, userData);

  glLinkProgram(progId);

#ifdef DEBUG
  if (!gkProgramIsValid(progId)) {
    gkProgramLogInfo(progId, stderr);
    exit(-1);
  }
#endif

  glUseProgram(progId);

  prog->shaders = shaders;

  prog->mvpi = glGetUniformLocation(progId, "MVP");
  prog->mvi  = glGetUniformLocation(progId, "MV");
  prog->nmi  = glGetUniformLocation(progId, "NM");
  prog->nmui = glGetUniformLocation(progId, "NMU");
  prog->vpi  = glGetUniformLocation(progId, "VP");
  prog->refc = 1;
  prog->updtLights    = 1;
  prog->updtMaterials = 1;

  glUniformBlockBinding(prog->progId,
                        glGetUniformBlockIndex(prog->progId, "JointBlock"),
                        1);

  return prog;
}

GkPipeline*
gkDefaultProgram() {
  GkPipeline *prog;
  GLuint     progId;
  GLuint     vert, frag;

  prog   = calloc(1, sizeof(*prog));
  progId = glCreateProgram();

  vert = gkShaderLoad(GL_VERTEX_SHADER,
                      gk_def_shader_vert(GK_DEF_SHADER_DEFAULT));
  frag = gkShaderLoad(GL_FRAGMENT_SHADER,
                      gk_def_shader_frag(GK_DEF_SHADER_DEFAULT));

  glAttachShader(progId, vert);
  glAttachShader(progId, frag);
  glLinkProgram(progId);

#ifdef DEBUG
  if (!gkProgramIsValid(progId)) {
    gkProgramLogInfo(progId, stderr);
    exit(-1);
  }
#endif

  glUseProgram(progId);

  glDeleteShader(vert);
  glDeleteShader(frag);

  prog->mvpi   = glGetUniformLocation(progId, "MVP");
  prog->mvi    = glGetUniformLocation(progId, "MV");
  prog->nmi    = glGetUniformLocation(progId, "NM");
  prog->nmui   = glGetUniformLocation(progId, "NMU");
  prog->vpi    = glGetUniformLocation(progId, "VP");
  prog->progId = progId;
  prog->refc   = 1;
  prog->updtLights    = 1;
  prog->updtMaterials = 1;

  return prog;
}

GLint
gkCurrentProgram() {
  GLint prog;
  glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
  return prog;
}

GkPipeline*
gkGetPipeline(char       *name,
              GkPipeline *(creatCb)(char *name, void *userData),
              void       *userData) {
  GkPipeline *prog;
  if ((prog = rb_find(gk_progs, (void *)name)))
    return prog;

  if (creatCb && (prog = creatCb(name, userData))) {
    rb_insert(gk_progs, (void *)strdup(name), prog);
    return prog;
  }

  return NULL;
}

GK_EXPORT
void
gkUseProgram(GkContext *ctx,
             GkPipeline *prog) {
  if (ctx->currState->prog == prog)
    return;

  glUseProgram(prog->progId);
  ctx->currState->prog = prog;
}

void
gk_prog_init() {
  gk_progs = rb_newtree_str();
}

void
gk_prog_deinit() {
  rb_destroy(gk_progs);
}
