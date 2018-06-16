/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#ifndef animatable_h
#define animatable_h

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/animation.h"

typedef struct GkAnimatable {
  FList *animations;
} GkAnimatable;

GkAnimatable*
gkAnimatable(void *object);

GK_EXPORT
void
gkAddAnimation(GkScene     *scene,
               GkNode      *node,
               GkAnimation *anim);

#endif /* animatable_h */
