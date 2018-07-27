/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "def_prog.h"
#include "shader/def_shader.h"

GkPipeline * gk_def_prog_cube = NULL;

GkPipeline*
gk_prog_cube() {
  GkPipeline  *prog;
  GLuint      progId;
  GLuint      vert, frag;

  if (gk_def_prog_cube)
    return gk_def_prog_cube;

  prog   = calloc(1, sizeof(*prog));
  progId = glCreateProgram();

  vert = gkShaderLoad(GL_VERTEX_SHADER,
                      gk_def_shader_vert(GK_DEF_SHADER_CUBE));
  frag = gkShaderLoad(GL_FRAGMENT_SHADER,
                      gk_def_shader_frag(GK_DEF_SHADER_CUBE));

  glAttachShader(progId, vert);
  glAttachShader(progId, frag);
  glLinkProgram(progId);

#ifdef DEBUG
  if (!gkProgramIsValid(progId)) {
    gkProgramLogInfo(progId, stderr);
    exit(-1);
  }
#endif

  glDeleteShader(vert);
  glDeleteShader(frag);

  prog->mvpi   = glGetUniformLocation(progId, "MVP");
  prog->progId = progId;
  prog->refc   = 1;

  gk_def_prog_cube = prog;

  return prog;
}
