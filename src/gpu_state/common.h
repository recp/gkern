/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef src_gpustate_common_h
#define src_gpustate_common_h

#include "../../include/gk/gk.h"
#include "../../include/gk/gpu_state.h"
#include "../../include/gk/pass.h"

#include <ds/hash.h>

typedef enum GkGPUStateType {
  GK_GPUSTATE_DEPTH   = 1,
  GK_GPUSTATE_BLEND   = 2,
  GK_GPUSTATE_TEXTURE = 3
} GkGPUStateType;

typedef struct GkStateBase {
  struct GkStateBase *next;
  GkGPUStateType      type;
  int32_t             arrayIndex;
} GkStateBase;

typedef struct GkDepthState {
  GkStateBase base;
  bool        depthTest;
  GLenum      depthFunc;
} GkDepthState;

typedef struct GkBlendState {
  GkStateBase base;
  bool        blend;
  GLenum      sfactor;
  GLenum      dfactor;
} GkBlendState;

typedef struct GkTextureState {
  GkStateBase base;
  GLuint      texunit;
  GLenum      target;
  GLuint      texid;
} GkTextureState;

typedef struct GkGPUStates {
  GkDepthState    depthState;
  GkBlendState    blendState;
  GLuint          activeTex;
  GkTextureState *texStates;
  GkPassOut      *renderOutput;
  GkProgInfo     *pinfo;
} GkGPUStates;

typedef void (*GkGPUApplyStateFn)(GkContext * __restrict ctx,
                                  GkStateBase * __restrict st);
typedef GkStateBase* (*GkGPUStateCreatFn)(GkContext * __restrict ctx);

_gk_hide
void*
gkGetOrCreatState(GkContext * __restrict ctx,
                  GkGPUStateType         type);

_gk_hide
GkStateBase*
gkCreatStateFromCurrent(GkContext * __restrict ctx,
                        GkGPUStateType         type);

_gk_hide
void*
gkGetOrCreatTexState(GkContext * __restrict ctx,
                     uint32_t               arrayIndex,
                     GLenum                 target);

_gk_hide
GkStateBase*
gkCreatTexStateFromCurrent(GkContext * __restrict ctx,
                           uint32_t               index,
                           GLenum                 target);
#endif /* src_gpustate_common_h */
