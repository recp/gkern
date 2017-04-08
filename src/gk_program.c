/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk-program.h"
#include "../include/gk-shader.h"
#include "default/shader/gk_def_shader.h"

#include <stdlib.h>
#include <string.h>

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
gkNewProgram(GLuint vertShader,
             GLuint fragShader,
             GLint  modelViewProjection,
             GLint  modelView,
             GLint  normalMatrix,
             GLint  useNormalMatrix) {
  static GkProgInfo *pinfo = NULL;
  GLuint program;

  if (pinfo)
    return pinfo;

  pinfo   = calloc(sizeof(*pinfo), 1);
  program = glCreateProgram();

  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

#ifdef DEBUG
  if (!gkProgramIsValid(program)) {
    gkProgramLogInfo(program, stderr);
    exit(-1);
  }
#endif

  glUseProgram(program);

  pinfo->mvpi = modelViewProjection;
  pinfo->mvi  = modelView;
  pinfo->nmi  = normalMatrix;
  pinfo->nmui = useNormalMatrix;
  pinfo->prog = program;
  pinfo->refc = 1;
  
  return pinfo;
}

GkProgInfo*
gkDefaultProgram() {
  static GkProgInfo *pinfo = NULL;
  GLuint program;
  GLuint vert, frag;

  if (pinfo)
    return pinfo;

  pinfo   = calloc(sizeof(*pinfo), 1);
  program = glCreateProgram();

  vert = gkShaderLoad(GL_VERTEX_SHADER,   gk_def_shader_vert());
  frag = gkShaderLoad(GL_FRAGMENT_SHADER, gk_def_shader_frag());

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

  return pinfo;
}
