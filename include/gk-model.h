/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_model_h
#define gk_model_h

#include "gk-common.h"
#include "gk-program.h"
#include "gk-material.h"

struct GkModel;
struct GkModelInst;
struct GkContext;

typedef void (*gkOnDraw)(struct GkModel     * model,
                         struct GkModelInst * instance,
                         bool finished);

typedef void (*gkOnClick)(struct GkModelInst * instance,
                          float x,
                          float y);

typedef struct GkFinalMatrix {
  uint32_t refc;
  uint32_t usenm; /* use generated normal matrix when non-uniform scaled */
  mat4     cmvp;  /* model view projection matrix */
  mat4     cmv;   /* model view matrix */
  mat4     cnmat; /* cached normal matrix */
} GkFinalMatrix;

/* some geometries or nodes may not have matrix, 
   so they will use parent's one. */
typedef struct GkMatrix {
  uint32_t       refc;
  uint32_t       cmatIsValid;
  /* 
   cached MVP and normal matrix: proj * view * model (GkMatrix->cmat)
   because multiple models may share same mvp matrix, to save space we use
   pointer space in every matrix, GkMatrix is allocating by malloc so to meet
   aligment requirements this space will be wasted anyway.
   
   dont change order because mat4 requires 16 byte aligment!
   */
  GkFinalMatrix *fmat;
  mat4           matrix;
  mat4           cmat;  /* cached matrix multiplied with parent */
} GkMatrix;

typedef struct GkGLEvents {
  gkOnDraw  onDraw;
  gkOnClick onClick;
} GkGLEvents;

typedef struct GkModelInstList {
  struct GkModelInst *instance;
  uint64_t            instanceCount;
} GkModelInstList;

typedef struct GkBuffer {
  GLuint  vbo;
  GLsizei size;
  GLuint  semantic;
  GLenum  usage;
  GLenum  type;
  GLenum  target;

  struct GkBuffer *prev;
  struct GkBuffer *next;
} GkBuffer;

typedef struct GkPrimitive {
  GLuint      flags;
  GLuint      vao;
  GkBuffer   *bufs;
  GkMaterial *material;
  GLsizei     bufc;
  GLsizei     count;
  GLenum      mode;

  struct GkPrimitive *prev;
  struct GkPrimitive *next;
} GkPrimitive;

typedef struct GkModel {
  GkPrimitive     *prim;
  GkProgInfo      *pinfo;
  GkMaterial      *material;
  GkGLEvents      *events;
  GkModelInstList *instances; /* TODO: */
  uint32_t         flags;
  uint32_t         primc;
} GkModel;

typedef struct GkModelInst {
  GkModel            *model;
  GkMatrix           *matrix;
  GkMaterial        *material; /* instances may use different materials */
  struct GkModelInst *next;
  uint64_t            flags;
  char                data[];   /* private field */
} GkModelInst;

void
gk_model_add(struct GkContext * __restrict ctx,
             GkModel * __restrict model,
             void    * __restrict source);

GkModel*
gk_model_find(struct GkContext * __restrict ctx,
              void * __restrict source);

#endif /* gk_model_h */
