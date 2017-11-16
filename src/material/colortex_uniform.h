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
gkUniformColorOrTex(GkColorOrTex * __restrict crtx,
                    char         * __restrict name,
                    GkProgram    * __restrict prog,
                    uint32_t     * __restrict texUnit);

void
gkUniformColorOrTexBuff(GkColorOrTex * __restrict crtx,
                        char         * __restrict buf,
                        char         * __restrict name,
                        GkProgram    * __restrict prog,
                        uint32_t     * __restrict texUnit);

#endif /* gk_colortex_uniform_h */
