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
  GkGPUBuffer *buff;
  uint32_t     vPOSITION, vTEXCOORD;

  if (gk__texquad)
    return;

  gk__texquad = prim = calloc(1, sizeof(*prim));

  buff         = calloc(1, sizeof(*buff));
  buff->size   = sizeof(gk__verts_quad);
  buff->target = GL_ARRAY_BUFFER;
  buff->type   = GL_FLOAT;
  buff->usage  = GL_STATIC_DRAW;

  prim->count  = 6;
  prim->flags  = GK_DRAW_ARRAYS;
  prim->mode   = GL_TRIANGLES;
  prim->bufc   = 1;
  prim->bufs   = buff;

  vPOSITION    = 0;
  vTEXCOORD    = 1;

  glGenVertexArrays(1, &prim->vao);
  glBindVertexArray(prim->vao);

  glGenBuffers(1, &buff->vbo);
  glBindBuffer(buff->target, buff->vbo);
  glBufferData(buff->target,
               buff->size,
               gk__verts_quad,
               buff->usage);

  glEnableVertexAttribArray(vPOSITION);
  glVertexAttribPointer(vPOSITION,
                        2,
                        buff->type,
                        GL_FALSE,
                        4 * sizeof(GLfloat),
                        NULL);

  glEnableVertexAttribArray(vTEXCOORD);
  glVertexAttribPointer(vTEXCOORD,
                        2,
                        buff->type,
                        GL_FALSE,
                        4 * sizeof(GLfloat),
                        (void*)(2 * sizeof(GLfloat)));

  glBindBuffer(buff->target, 0);
  glBindVertexArray(0);
}
