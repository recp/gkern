/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef uniform_h
#define uniform_h

#include "common.h"

struct GkProgram;

GK_EXPORT
void
gkUniform1i(struct GkProgram * __restrict prog,
            const char       * __restrict name,
            GLint                         val);

GK_EXPORT
void
gkUniform1ui(struct GkProgram * __restrict prog,
             const char       * __restrict name,
             GLuint                        val);

GK_EXPORT
void
gkUniform1f(struct GkProgram  * __restrict prog,
            const char        * __restrict name,
            float                          val);

GK_EXPORT
void
gkUniform2f(struct GkProgram  * __restrict prog,
            const char        * __restrict name,
            float                          val[2]);

#endif /* uniform_h */
