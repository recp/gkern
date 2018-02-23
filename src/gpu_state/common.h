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
  GK_GPUSTATE_DEPTH      = 1,
  GK_GPUSTATE_BLEND      = 2,
  GK_GPUSTATE_TEXTURE    = 3,
  GK_GPUSTATE_RENDER_OUT = 4,
  GK_GPUSTATE_CULLFACE   = 5
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
  GLboolean   mask;
} GkDepthState;

typedef struct GkFaceState {
  GkStateBase base;
  bool        cull;
  GLenum      face;
  GLenum      frontFace;
} GkFaceState;

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

typedef struct GkRenderOutState {
  GkStateBase base;
  GkPassOut  *renderOutput;
} GkRenderOutState;

typedef struct GkStatesItem {
  FListItem *states;
  bool       isempty;
} GkStatesItem;

typedef struct GkGPUStates {
  GkDepthState     depthState;
  GkBlendState     blendState;
  GkRenderOutState outputState;
  GkFaceState      faceState;
  GLuint           activeTex;
  GkTextureState  *texStates;
  GkProgram       *prog;
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
gkCreatState(GkContext    * __restrict ctx,
             GkStatesItem * __restrict sti,
             GkGPUStateType            type);

_gk_hide
void*
gkGetOrCreatTexState(GkContext * __restrict ctx,
                     uint32_t               arrayIndex,
                     GLenum                 target);

_gk_hide
GkStateBase*
gkCreatTexState(GkContext    * __restrict ctx,
                GkStatesItem * __restrict sti,
                uint32_t                  index,
                GLenum                    target);

_gk_hide
void
gkStateMakeCurrent(GkContext   * __restrict ctx,
                   GkStateBase * __restrict st);

#endif /* src_gpustate_common_h */
