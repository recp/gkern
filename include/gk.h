/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_h
#define gk_h

#include "gk-common.h"
#include "gk-shader.h"
#include "gk-program.h"
#include <cglm.h>
#include <stdlib.h>
#include <stdbool.h>

#define GK_DRAW_ELEMENTS (1 << 0)
#define GK_DRAW_ARRAYS   (1 << 1)
#define GK_COMPLEX       (1 << 2)

struct GkModelBase;
struct GkModel;
struct GkModelInstance;

typedef void (*gkOnDraw)(struct GkModelBase * model,
                         struct GkModelInstance * instance,
                         bool finished);

typedef void (*gkOnClick)(struct GkModelInstance * instance,
                          float x,
                          float y);

typedef struct GkModelBase {
  void    *data;
  gkOnDraw onDraw;
  int32_t  flags;
} GkModelBase;

typedef struct GkModel {
  GkModelBase base;
  GLuint  *vbo;
  GLuint  *tex;
  GLuint   vao;
  uint32_t vboCount;
  GLsizei  count;
  GLenum   mode;
} GkModel;

typedef struct GkComplexModel {
  GkModelBase base;
  GLuint     *vao;
  GLuint     *vbo;
  GLuint     *tex;
  GLsizei    *count;
  GLenum     *modes;
  GLuint      vaoCount;
  uint32_t    vboCount;
} GkComplexModel;

typedef struct GkModelInstance {
  GkModelBase *model;
  void        *data;
  gkOnDraw     onDraw;
  gkOnClick    onClick;
  vec4        *matrix;
  GLint        matrixLoc;
  int32_t      flags;
  struct GkModelInstance *next;
} GkModelInstance;

GkModelInstance *
gkMakeInstance(GkModelBase *model,
               mat4 matrix,
               GLint matrixLoc);

void
gkRender(GkModelInstance * modelInstance);

#endif /* gk_h */
