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
#include "../../include/gk/prims/builtin-prim.h"
#include "../../include/gk/gk.h"
#include <limits.h>

static
_gk_hide
void
gkPrimInitTexQuad(void);

static const GLfloat gk__verts_quad[] = {
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f,
   -1.0f,  1.0f,  0.0f, 1.0f,

   -1.0f,  1.0f,  0.0f, 1.0f,
   -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f
};

GkPrimitive *gk__texquad = NULL;

GK_EXPORT
GkPrimitive*
gkPrimGetTexQuad() {
  if (!gk__texquad)
    gkPrimInitTexQuad();

  return gk__texquad;
}

static
_gk_hide
void
gkPrimInitTexQuad() {
  GkPrimitive *prim;
  GkGpuBuffer *buff;
  uint32_t     vPOSITION, vTEXCOORD;

  if (gk__texquad)
    return;

  gk__texquad = prim = calloc(1, sizeof(*prim));

  buff = gkGpuBufferNew(NULL, GK_ARRAY, sizeof(gk__verts_quad));
  gkGpuBufferFeed(buff, GK_STATIC_DRAW, gk__verts_quad);

  prim->count  = 6;
  prim->flags  = GK_DRAW_ARRAYS;
  prim->mode   = GL_TRIANGLES;
  prim->bufc   = 1;
  prim->bufs   = buff;

  vPOSITION    = 0;
  vTEXCOORD    = 1;

  glGenVertexArrays(1, &prim->vao);
  glBindVertexArray(prim->vao);

  glEnableVertexAttribArray(vPOSITION);
  glVertexAttribPointer(vPOSITION,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        4 * sizeof(GLfloat),
                        NULL);

  glEnableVertexAttribArray(vTEXCOORD);
  glVertexAttribPointer(vTEXCOORD,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        4 * sizeof(GLfloat),
                        (void*)(2 * sizeof(GLfloat)));

  glBindBuffer(buff->target, 0);
  glBindVertexArray(0);
}
