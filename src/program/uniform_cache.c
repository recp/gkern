/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "uniform_cache.h"

GK_EXPORT
GLint
gkUniformLoc(GkProgInfo *pinfo, const char *name) {
  void *found;
  GLint loc;

  if (!pinfo->uniforms)
    pinfo->uniforms = hash_new_str(8);

  if ((found = hash_get(pinfo->uniforms, (void *)name)))
    return ((GLint)found) - 1;

  loc = glGetUniformLocation(pinfo->prog, name);
  hash_set(pinfo->uniforms,
           (void *)name,
           (void *)(uintptr_t)(loc + 1));

  return loc;
}
