/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */


#ifndef uniform_h
#define uniform_h

#include "common.h"

struct GkProgInfo;

GK_EXPORT
void
gkUniform1i(struct GkProgInfo * __restrict pinfo,
            const char        * __restrict name,
            GLint                          val);

#endif /* uniform_h */
