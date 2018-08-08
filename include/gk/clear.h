/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_clear_h
#define gk_clear_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

struct GkScene;

typedef enum GkClearEffect {
  GK_CLEAR_EFFECT_NONE     = 0,
  GK_CLEAR_EFFECT_GRADIENT = 1
} GkClearEffect;

GK_EXPORT
void
gkClearEffect(GkScene *scene, GkPipeline *pip);

GK_EXPORT
void
gkSetClearEffect(GkScene *scene, GkClearEffect effect);

#ifdef __cplusplus
}
#endif
#endif /* gk_clear_h */
