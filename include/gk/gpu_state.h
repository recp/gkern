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
gkDepthMask(GkContext * __restrict ctx, GLboolean flag);

GK_EXPORT
void
gkDepthFunc(GkContext * __restrict ctx, GLenum func);

GK_EXPORT
void
gkDisableCullFace(GkContext * __restrict ctx);

GK_EXPORT
void
gkEnableCullFace(GkContext * __restrict ctx);

GK_EXPORT
void
gkToggleDoubleSided(GkContext * __restrict ctx, bool doubleSided);

GK_EXPORT
void
gkCullFace(GkContext * __restrict ctx, GLenum face);

GK_EXPORT
void
gkFrontFace(GkContext * __restrict ctx, GLenum face);

GK_EXPORT
void
gkEnableBlend(GkContext * __restrict ctx);

GK_EXPORT
void
gkDisableBlend(GkContext * __restrict ctx);

GK_EXPORT
void
gkBlendFunc(GkContext * __restrict ctx, GLenum src, GLenum dst);

GK_EXPORT
void
gkEnableBlendi(GkContext * __restrict ctx, GLuint buf);

GK_EXPORT
void
gkDisableBlendi(GkContext * __restrict ctx, GLuint buf);

GK_EXPORT
void
gkBlendFunci(GkContext * __restrict ctx,
             GLint                  buf,
             GLenum                 src,
             GLenum                 dst);
GK_EXPORT
void
gkSrcBuff(GkContext * __restrict ctx, GLenum buff);

GK_EXPORT
void
gkDstBuff(GkContext * __restrict ctx, GLenum buff);

GK_EXPORT
void
gkBlendEq(GkContext * __restrict ctx, GLenum eq);

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

GK_EXPORT
void
gkBindPrimitive(GkPrimitive * __restrict prim);

GK_EXPORT
void
gkBindPrimInst(GkPrimInst * __restrict primInst);

#endif /* gpu_state_h */
