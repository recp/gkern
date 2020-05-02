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
#include "builtin_shader.h"
#include "shader.h"

GkPipeline*
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
    case GK_BUILTIN_PROG_WEIGBL_COMPOS: {
      const char *src[2];
      GLenum      typ[2] = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER
      };

      src[0] =
#include "glsl/vert/rtt.glsl"
      ;

      src[1] =
#include "glsl/frag/wboit_compos.glsl"
      ;

      return gkGetOrCreatProgByName("wboit_compos",
                                    src,
                                    typ,
                                    2,
                                    GK_SHADER_FLAG_MVP);
    }
    case GK_BUILTIN_PROG_CLR_GRAD_CIRC: {
      const char *src[2];
      GLenum      typ[2] = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER
      };

      src[0] =
#include "glsl/vert/plain.glsl"
      ;

      src[1] =
#include "glsl/frag/clr_grad_circ.glsl"
      ;

      return gkGetOrCreatProgByName("clr_grad_circ", src, typ, 2, 0);
    }
    default:
      break;
  }
  return NULL;
}
