/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_vertex_h
#define gk_vertex_h

#include "gk-common.h"
#include "gk-program.h"

/* same as AkInputSemantic but AssetKit is not required! */

typedef enum GkAttribType {
  /* read semanticRaw */
  GK_ATTRIB_TYPE_OTHER           = 0,
  GK_ATTRIB_TYPE_BINORMAL        = 1,
  GK_ATTRIB_TYPE_COLOR           = 2,
  GK_ATTRIB_TYPE_CONTINUITY      = 3,
  GK_ATTRIB_TYPE_IMAGE           = 4,
  GK_ATTRIB_TYPE_INPUT           = 5,
  GK_ATTRIB_TYPE_IN_TANGENT      = 6,
  GK_ATTRIB_TYPE_INTERPOLATION   = 7,
  GK_ATTRIB_TYPE_INV_BIND_MATRIX = 8,
  GK_ATTRIB_TYPE_JOINT           = 9,
  GK_ATTRIB_TYPE_LINEAR_STEPS    = 10,
  GK_ATTRIB_TYPE_MORPH_TARGET    = 11,
  GK_ATTRIB_TYPE_MORPH_WEIGHT    = 12,
  GK_ATTRIB_TYPE_NORMAL          = 13,
  GK_ATTRIB_TYPE_OUTPUT          = 14,
  GK_ATTRIB_TYPE_OUT_TANGENT     = 15,
  GK_ATTRIB_TYPE_POSITION        = 16,
  GK_ATTRIB_TYPE_TANGENT         = 17,
  GK_ATTRIB_TYPE_TEXBINORMAL     = 18,
  GK_ATTRIB_TYPE_TEXCOORD        = 19,
  GK_ATTRIB_TYPE_TEXTANGENT      = 20,
  GK_ATTRIB_TYPE_UV              = 21,
  GK_ATTRIB_TYPE_VERTEX          = 22,
  GK_ATTRIB_TYPE_WEIGHT          = 23
} GkAttribType;

typedef struct GkVertexAttrib {
  const GLchar *name;
  GLint         index;
  GkAttribType  type;
} GkVertexAttrib;

GLint
gk_vertAttribIndex(const char *name);

GLint
gk_vertAddAttrib(const char *name);

GLint
gk_vertRemoveAttrib(const char *name);

void
gk_vertSetAttribs(size_t      count,
                  const char *names[]);

#endif /* gk_vertex_h */
