/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/program.h"
#include "../../include/gk/shader.h"
#include "../default/shader/def_shader.h"

#include "program.h"
#include "../gpu_state/common.h"
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

GkProgram*
gkMakeProgram(GkShader *shaders,
              void (*beforeLinking)(GkProgram *prog, void *data),
              void *userData) {
  static GkProgram *prog;
  GLuint progId;
  
  prog = calloc(sizeof(*prog), 1);
  prog->progId = progId = glCreateProgram();

  gkAttachShaders(progId, shaders);
  
  if (beforeLinking)
    beforeLinking(prog, userData);
  
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
  prog->refc = 1;
  prog->updtLights    = 1;
  prog->updtMaterials = 1;

  return prog;
}

GkProgram*
gkDefaultProgram() {
  GkProgram *prog;
  GLuint     progId;
  GLuint     vert, frag;

  prog   = calloc(sizeof(*prog), 1);
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

GkProgram*
gkGetOrCreatProg(char      *name,
                 GkProgram *(creatCb)(char *name, void *userData),
                 void      *userData) {
  GkProgram *prog;
  if ((prog = rb_find(gk_progs, (void *)name)))
    return prog;

  if ((prog = creatCb(name, userData))) {
    rb_insert(gk_progs, (void *)strdup(name), prog);
    return prog;
  }

  return NULL;
}

GK_EXPORT
void
gkUseProgram(GkContext *ctx,
             GkProgram *prog) {
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
