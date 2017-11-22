/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "builtin_shader.h"
#include "shader.h"

GkProgram*
gkBuiltinProg(GkBuiltinProg progtype) {
  switch (progtype) {
    case GK_BUILTIN_PROG_RTT: {
      char *vert, *frag;

      vert =
#include "glsl/vert/rtt.glsl"
      ;

      frag =
#include "glsl/frag/rtt.glsl"
      ;

      return gkGetOrCreatProgByName("builtin_rtt",
                                    (const char *[]) {
                                      vert,
                                      frag
                                    },
                                    (GLenum[]) {
                                      GL_VERTEX_SHADER,
                                      GL_FRAGMENT_SHADER
                                    },
                                    2,
                                    GK_SHADER_FLAG_BUILTIN);
    }
    default:
      break;
  }
  return NULL;
}
