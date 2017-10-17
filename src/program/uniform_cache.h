/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef src_uniform_cache_h
#define src_uniform_cache_h

#include "../../include/gk/program.h"
#include "../../include/gk/gk.h"

GK_EXPORT
GLint
gkUniformLoc(GkProgInfo *pinfo, const char *name);

#endif /* src_uniform_cache_h */

