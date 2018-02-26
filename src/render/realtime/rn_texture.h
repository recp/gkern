/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef rn_texture_h
#define rn_texture_h

#include "../../../include/gk/gk.h"
#include "../../../include/gk/pass.h"

void
gkRenderTexture(GkScene * __restrict scene, GkPass * __restrict pass);

void
gkBlit(GkScene  * __restrict scene,
       GkOutput * __restrict src,
       GkOutput * __restrict dst,
       int32_t               colorIndex);

#endif /* rn_texture_h */
