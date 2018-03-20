/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_colortex_uniform_h
#define gk_colortex_uniform_h

#include "../../include/gk/gk.h"

void
gkUniformColor(GkColor   * __restrict color,
               char      * __restrict name,
               GkProgram * __restrict prog);

void
gkUniformTex(GkContext  * __restrict ctx,
             GkMaterial * __restrict mat,
             GkTexture  * __restrict tex,
             char       * __restrict name,
             GkProgram  * __restrict prog);

void
gkUniformColorOrTex(GkContext    * __restrict ctx,
                    GkMaterial   * __restrict mat,
                    GkColorOrTex * __restrict crtx,
                    char         * __restrict name,
                    GkProgram    * __restrict prog);

void
gkUniformColorBuff(GkColor   * __restrict color,
                   char      * __restrict buf,
                   char      * __restrict name,
                   GkProgram * __restrict prog);

void
gkUniformColorOrTexBuff(GkContext    * __restrict ctx,
                        GkMaterial   * __restrict mat,
                        GkColorOrTex * __restrict crtx,
                        char         * __restrict buf,
                        char         * __restrict name,
                        GkProgram    * __restrict prog);

#endif /* gk_colortex_uniform_h */
