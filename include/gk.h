/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_h
#define gk_h
#ifdef __cplusplus
extern "C" {
#endif

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

struct GkModelInstance;

typedef struct GkModelBase {
  struct GkModelInstance *instance; /* model instance list        */
  mat4     matrix;                  /* original model transform   */
  GLint    matrixLoc;
  void    *data;                    /* user defined/attached data */
  gkOnDraw onDraw;
  int32_t  flags;
} GkModelBase;

typedef struct GkModel {
  GkModelBase base;
  GLuint     *vbo;
  GLuint     *tex;
  GLuint      vao;
  uint32_t    vboCount;
  GLsizei     count;
  GLenum      mode;
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
  mat4         matrix;
  GLint        matrixLoc;
  int32_t      flags;
  struct GkModelInstance *next;
} GkModelInstance;

typedef struct GkNode {
  GkModelInstance *model;
  struct GkNode   *next;
  struct GkNode   *chld;
} GkNode;

GkModelInstance *
gkMakeInstance(GkModelBase *model,
               GLint matrixLoc);

void
gkModelEmptyMatrix(GkModelBase *modelBase);

void
gkUniformModelMatrix(GkModelBase *modelBase);

void
gkUniformInstanceMatrix(GkModelInstance *modelInstance);

void
gkRenderInstance(GkModelInstance * modelInstance);

void
gkRenderModel(GkModelBase * model);

#ifdef __cplusplus
}
#endif
#endif /* gk_h */
