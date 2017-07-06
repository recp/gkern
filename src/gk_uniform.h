/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_uniform_h
#define gk_uniform_h

#include "../include/gk/gk.h"

GLint
gkGetUniformLoc(GLuint prog,
                char  * __restrict buf,
                char  * __restrict name);

#endif /* gk_uniform_h */
