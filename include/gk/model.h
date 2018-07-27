/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_model_h
#define gk_model_h

#include "common.h"
#include "program.h"
#include "material.h"
#include "bbox.h"
#include "transform.h"
#include "buffer.h"

struct GkModel;
struct GkModelInst;
struct GkContext;
struct FListItem;

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

typedef struct GkPrimitive {
  struct GkPrimitive *prev;
  struct GkPrimitive *next;
  GkMaterial         *material;
  GkMaterial         *activeMaterial;
  GkGPUBuffer        *bufs;
  struct FListItem   *inputs;
  GkBBox              bbox; /* local */
  GLuint              flags;
  GLuint              vao;
  GLsizei             bufc;
  GLsizei             count;
  GLenum              mode;
} GkPrimitive;

typedef struct GkPrimInst {
  GkPrimitive *prim;
  GkMaterial  *material;
  GkMaterial  *activeMaterial;
  GkTransform *trans;
  GkBBox       bbox;
} GkPrimInst;

typedef enum GkModelFlags {
  GK_MODEL_FLAGS_NONE      = 0,
  GK_MODEL_FLAGS_DRAW_BBOX = 1 << 0,
} GkModelFlags;

typedef struct GkModel {
  GkPipeline       *prog;
  GkMaterial      *material;
  GkGLEvents      *events;
  GkModelInstList *instances; /* TODO: */
  GkBBox           bbox;   /* local */
  vec3             center;
  uint32_t         flags;
  uint32_t         primc;
  GkPrimitive      prims[];
} GkModel;

typedef struct GkModelInst {
  struct GkModelInst *next;
  GkModel            *model;
  GkTransform        *trans;    /* readonly: don't set this manually */
  GkMaterial         *material; /* instances may use different materials */
  GkMaterial         *activeMaterial;
  GkBBox              bbox;
  vec3                center;
  bool                addedToScene;
  uint64_t            flags;
  int32_t             primc;
  GkPrimInst          prims[];
} GkModelInst;

void
gk_model_add(struct GkContext * __restrict ctx,
             GkModel * __restrict model,
             void    * __restrict source);

GkModel*
gk_model_find(struct GkContext * __restrict ctx,
              void * __restrict source);

GkModelInst*
gkMakeInstance(GkModel *model);

#endif /* gk_model_h */
