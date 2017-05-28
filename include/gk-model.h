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
#include "gk-bbox.h"
#include "gk-transform.h"

struct GkModel;
struct GkModelInst;
struct GkContext;

typedef void (*gkOnDraw)(struct GkModel     * model,
                         struct GkModelInst * instance,
                         bool finished);

typedef void (*gkOnClick)(struct GkModelInst * instance,
                          float x,
                          float y);

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

typedef struct GkPrimInst {
  GkPrimitive *prim;
  GkMaterial  *material;
} GkPrimInst;

typedef enum GkModelFlags {
  GK_MODEL_FLAGS_NONE      = 0,
  GK_MODEL_FLAGS_DRAW_BBOX = 1 << 0,
} GkModelFlags;

typedef struct GkModel {
  GkPrimitive     *prim;
  GkProgInfo      *pinfo;
  GkMaterial      *material;
  GkGLEvents      *events;
  GkModelInstList *instances; /* TODO: */
  GkBBox          *bbox;
  uint32_t         flags;
  uint32_t         primc;
} GkModel;

typedef struct GkModelInst {
  GkModel            *model;
  GkTransform        *trans;
  RBTree             *prims;    /* to customize each primitive material  */
  GkMaterial         *material; /* instances may use different materials */
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
