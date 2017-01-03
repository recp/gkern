/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_model_h
#define gk_model_h

struct GkModelBase;
struct GkModel;
struct GkModelInst;

typedef void (*gkOnDraw)(struct GkModelBase * model,
                         struct GkModelInst * instance,
                         bool finished);

typedef void (*gkOnClick)(struct GkModelInst * instance,
                          float x,
                          float y);

/* for save space we separate the matrix */
typedef struct GkMatrix {
  mat4  matrix;
  GLint index;
} GkMatrix;

typedef struct GkGLEvents {
  gkOnDraw  onDraw;
  gkOnClick onClick;
} GkGLEvents;

typedef struct GkModelInstList {
  struct GkModelInst *instance;
  uint64_t            instanceCount;
} GkModelInstList;

typedef struct GkModelBase {
  uint64_t         flags;
  GkMatrix         cachedMatrix;
  uint32_t         cachedMatrixIsValid;
  GkMatrix        *matrix;
  GkGLEvents      *events;
  GkModelInstList *instances;
} GkModelBase;

typedef struct GkModel {
  GkModelBase base;
  GLuint     *vbo;
  GLuint      vao;
  GLsizei     vboCount;
  GLsizei     count;
  GLenum      mode;
} GkModel;

typedef struct GkComplexModel {
  GkModelBase base;
  GLuint     *vao;
  GLuint     *vbo;
  GLsizei    *count;
  GLenum     *modes;
  GLsizei     vaoCount;
  GLsizei     vboCount;
} GkComplexModel;

typedef struct GkModelInst {
  GkModelBase        *model;
  GkMatrix           *matrix;
  GkGLEvents         *events;
  struct GkModelInst *next;
  uint64_t            flags;
  char                data[]; /* private field */
} GkModelInst;

#endif /* gk_model_h */
