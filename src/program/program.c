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

GkProgInfo*
gkNewProgram(GkShader *shaders) {
  static GkProgInfo *pinfo;
  GLuint program;

  pinfo   = calloc(sizeof(*pinfo), 1);
  program = glCreateProgram();

  gkAttachShaders(program, shaders);
  glLinkProgram(program);

#ifdef DEBUG
  if (!gkProgramIsValid(program)) {
    gkProgramLogInfo(program, stderr);
    exit(-1);
  }
#endif

  glUseProgram(program);

  pinfo->mvpi = glGetUniformLocation(program, "MVP");
  pinfo->mvi  = glGetUniformLocation(program, "MV");
  pinfo->nmi  = glGetUniformLocation(program, "NM");
  pinfo->nmui = glGetUniformLocation(program, "NMU");
  pinfo->prog = program;
  pinfo->refc = 1;
  pinfo->updtLights    = 1;
  pinfo->updtMaterials = 1;

  return pinfo;
}

GkProgInfo*
gkDefaultProgram() {
  GkProgInfo *pinfo;
  GLuint      program;
  GLuint      vert, frag;

  pinfo   = calloc(sizeof(*pinfo), 1);
  program = glCreateProgram();

  vert = gkShaderLoad(GL_VERTEX_SHADER,
                      gk_def_shader_vert(GK_DEF_SHADER_DEFAULT));
  frag = gkShaderLoad(GL_FRAGMENT_SHADER,
                      gk_def_shader_frag(GK_DEF_SHADER_DEFAULT));

  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);

#ifdef DEBUG
  if (!gkProgramIsValid(program)) {
    gkProgramLogInfo(program, stderr);
    exit(-1);
  }
#endif

  glUseProgram(program);

  glDeleteShader(vert);
  glDeleteShader(frag);

  pinfo->mvpi = glGetUniformLocation(program, "MVP");
  pinfo->mvi  = glGetUniformLocation(program, "MV");
  pinfo->nmi  = glGetUniformLocation(program, "NM");
  pinfo->nmui = glGetUniformLocation(program, "NMU");
  pinfo->prog = program;
  pinfo->refc = 1;
  pinfo->updtLights    = 1;
  pinfo->updtMaterials = 1;
  
  return pinfo;
}

GLint
gkCurrentProgram() {
  GLint prog;
  glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
  return prog;
}

GkProgInfo*
gkGetOrCreatProg(char       *name,
                 GkProgInfo *(creatCb)(char *name, void *userData),
                 void       *userData) {
  GkProgInfo *pinfo;
  if ((pinfo = rb_find(gk_progs, (void *)name)))
    return pinfo;

  if ((pinfo = creatCb(name, userData))) {
    rb_insert(gk_progs, (void *)strdup(name), pinfo);
    return pinfo;
  }

  return NULL;
}

GK_EXPORT
void
gkUseProgram(GkContext  *ctx,
             GkProgInfo *pinfo) {
  if (ctx->currState->pinfo == pinfo)
    return;

  glUseProgram(pinfo->prog);
  ctx->currState->pinfo = pinfo;
}

void
gk_prog_init() {
  gk_progs = rb_newtree_str();
}

void
gk_prog_deinit() {
  rb_destroy(gk_progs);
}

