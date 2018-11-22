/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
