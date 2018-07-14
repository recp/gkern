/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_keyframe_animation_h
#define gk_keyframe_animation_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "value.h"

struct GkAnimation;
struct GkScene;
struct GkNode;

  typedef enum GkTargetPropertyType {
    GK_TARGET_UNKNOWN  = 0,
    GK_TARGET_X        = 1,
    GK_TARGET_Y        = 2,
    GK_TARGET_Z        = 3,
    GK_TARGET_XY       = 4,
    GK_TARGET_XYZ      = 5,
    GK_TARGET_ANGLE    = 6,
    GK_TARGET_POSITION = 7,
    GK_TARGET_SCALE    = 8,
    GK_TARGET_ROTATE   = 9,
    GK_TARGET_QUAT     = 10
  } GkTargetPropertyType;

typedef enum GkSamplerBehavior {
  GK_SAMPLER_UNDEFINED      = 0,
  GK_SAMPLER_CONSTANT       = 1,
  GK_SAMPLER_GRADIENT       = 2,
  GK_SAMPLER_CYCLE          = 3,
  GK_SAMPLER_OSCILLATE      = 4,
  GK_SAMPLER_CYCLE_RELATIVE = 5
} GkSamplerBehavior;

typedef struct GkAnimSampler {
  GkBuffer         *input;
  GkBuffer         *output;
  GkBuffer         *interp;
  GkBuffer         *inTangent;
  GkBuffer         *outTangent;
  GkSamplerBehavior preBehavior;
  GkSamplerBehavior postBehavior;
  GkInterpType      uniInterp;
} GkAnimSampler;

typedef struct GkChannel {
  struct GkChannel *next;
  GkAnimSampler    *sampler;
  GkNode           *node; /* Required if target is Node transform */
  void             *target;
  GkType            targetType;
  GkTargetPropertyType property;
  GkInterpType      lastInterp;
  uint32_t          keyIndex;
  GkValue           ov[2];
  GkValue           kv[2];
  GkValue           delta;
  double            beginTimeRef;
  double            endTimeRef;
  double            beginTime;
  double            endTime;
  double            duration;
  double            keyStartTime;
  double            keyEndTime;
  bool              isTransform:1;
  bool              isLocalTransform:1;
  bool              isPrepared:1;
  bool              isPreparedKey:1;
  bool              computeDelta:1;
} GkChannel;

typedef struct GkKeyFrameAnimation {
  GkAnimation  base;
  GkChannel   *channel;
  uint32_t     channelCount;
} GkKeyFrameAnimation;

typedef struct GkAnimationClip {
  GkAnimation *anim;
  double       start;
  double       end;
} GkAnimationClip;

GK_EXPORT
GkKeyFrameAnimation*
gkKeyFrameAnimation(void);

#ifdef __cplusplus
}
#endif
#endif /* gk_keyframe_animation_h */
