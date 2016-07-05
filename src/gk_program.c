/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk-program.h"

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
