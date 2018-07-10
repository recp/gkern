/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/animation.h"

_gk_hide
bool
gkBuiltinKeyAnim(GkAnimation *anim,
                 GkChannel   *channel,
                 GkValue     *to,
                 GkValue     *delta);

void
gkRunKFAnim(GkKeyFrameAnimation *kfa, float ease);

GK_EXPORT
void
gkPrepChannel(GkAnimation *anim, GkChannel *ch);

GK_EXPORT
void
gkPrepChannelKey(GkKeyFrameAnimation *anim, GkChannel *ch);
