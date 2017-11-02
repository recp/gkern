/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gpu_state_h
#define gpu_state_h

#include "common.h"
#include "context.h"

GK_EXPORT
void
gkPushState(GkContext * __restrict ctx);

GK_EXPORT
void
gkPopState(GkContext * __restrict ctx);

GK_EXPORT
void
gkEnableDepthTest(GkContext * __restrict ctx);

GK_EXPORT
void
gkDisableDepthTest(GkContext * __restrict ctx);

GK_EXPORT
void
gkActiveTexture(GkContext * __restrict ctx,
                uint32_t               unit);

GK_EXPORT
void
gkBindTextureTo(GkContext * __restrict ctx,
                uint32_t               unit,
                uint32_t               target,
                uint32_t               texid);

GK_EXPORT
void
gkBindTexture(GkContext * __restrict ctx,
              uint32_t               target,
              uint32_t               texid);

#endif /* gpu_state_h */
