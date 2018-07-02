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
} GkAnimSampler;

typedef struct GkChannel {
  GkAnimSampler    *sampler;
  const char       *target;
  struct AkChannel *next;
} GkChannel;

typedef struct GkKeyFrameAnimation {
  GkAnimation    base;
  GkAnimSampler *sampler;
  GkChannel     *channel;
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
