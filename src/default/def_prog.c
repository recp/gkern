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
#include "def_prog.h"
#include "shader/def_shader.h"

GkPipeline *gk_def_prog_cube     = NULL;
GkPipeline *gk_def_prog_drawbone = NULL;

GkPipeline*
gk_prog_cube() {
  GkPipeline *prog;
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

GkPipeline*
gk_prog_drawbone() {
  GkPipeline *prog;
  GLuint      progId;
  GLuint      vert, frag;

  if (gk_def_prog_drawbone)
    return gk_def_prog_drawbone;

  prog   = calloc(1, sizeof(*prog));
  progId = glCreateProgram();

  vert = gkShaderLoad(GL_VERTEX_SHADER,
                      gk_def_shader_vert(GK_DEF_SHADER_DRAWBONE));
  frag = gkShaderLoad(GL_FRAGMENT_SHADER,
                      gk_def_shader_frag(GK_DEF_SHADER_DRAWBONE));

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

  prog->mvpi   = -1;
  prog->progId = progId;
  prog->refc   = 1;

  glUniformBlockBinding(prog->progId,
                        glGetUniformBlockIndex(prog->progId, "JointBlock"),
                        2);

  gk_def_prog_drawbone = prog;

  return prog;
}
