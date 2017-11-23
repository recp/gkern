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
      const char *src[2];
      GLenum      typ[2] = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER
      };

      src[0] =
#include "glsl/vert/rtt.glsl"
      ;

      src[1] =
#include "glsl/frag/rtt.glsl"
      ;

      return gkGetOrCreatProgByName("builtin_rtt", src, typ, 2, 0);
    }

    case GK_BUILTIN_PROG_DEPTH: {
      const char *src[2];
      GLenum      typ[2] = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER
      };

      src[0] =
#include "glsl/vert/rtt.glsl"
      ;

      src[1] =
#include "glsl/frag/depth.glsl"
      ;

      return gkGetOrCreatProgByName("builtin_depth", src, typ, 2, 0);
    }
    case GK_BUILTIN_PROG_SHADOWMAP: {
      const char *src[2];
      GLenum      typ[2] = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER
      };

      src[0] =
#include "glsl/vert/shadowmap.glsl"
      ;

      src[1] =
#include "glsl/frag/shadowmap.glsl"
      ;

      return gkGetOrCreatProgByName("builtin_shdwmap",
                                    src,
                                    typ,
                                    2,
                                    GK_SHADER_FLAG_MVP);
    }
    default:
      break;
  }
  return NULL;
}
