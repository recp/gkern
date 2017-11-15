/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "def_prog.h"
#include "shader/def_shader.h"

GkProgram * gk_def_prog_cube = NULL;

GkProgram*
gk_prog_cube() {
  GkProgram  *prog;
  GLuint      program;
  GLuint      vert, frag;

  if (gk_def_prog_cube)
    return gk_def_prog_cube;

  prog    = calloc(sizeof(*prog), 1);
  program = glCreateProgram();

  vert = gkShaderLoad(GL_VERTEX_SHADER,
                      gk_def_shader_vert(GK_DEF_SHADER_CUBE));
  frag = gkShaderLoad(GL_FRAGMENT_SHADER,
                      gk_def_shader_frag(GK_DEF_SHADER_CUBE));

  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);

#ifdef DEBUG
  if (!gkProgramIsValid(program)) {
    gkProgramLogInfo(program, stderr);
    exit(-1);
  }
#endif

  glDeleteShader(vert);
  glDeleteShader(frag);

  prog->mvpi = glGetUniformLocation(program, "MVP");
  prog->prog = program;
  prog->refc = 1;

  gk_def_prog_cube = prog;

  return prog;
}
