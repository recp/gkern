/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "uniform_cache.h"
#include <string.h>

GK_EXPORT
GLint
gkUniformLoc(GkProgram *prog, const char *name) {
  void *found;
  GLint loc;

  if (!prog->uniforms)
    prog->uniforms = hash_new_str(8);

  if ((found = hash_get(prog->uniforms, (void *)name)))
    return ((GLint)found) - 1;

  loc = glGetUniformLocation(prog->progId, name);
  hash_set(prog->uniforms,
           (void *)strdup(name),
           (void *)(uintptr_t)(loc + 1));

  return loc;
}

GLint
gkUniformLocBuff(GkProgram * __restrict prog,
                 char      * __restrict name,
                 char      * __restrict buf) {
  char  *pBuf;
  size_t attrlen;
  GLint  loc;
  
  if (!buf)
    return gkUniformLoc(prog, name);
  
  pBuf = strrchr(buf, '.') + 1;
  strcpy(pBuf, name);
  
  attrlen = strlen(pBuf);
  loc     = gkUniformLoc(prog, buf);
  memset((buf + strlen(buf)) - attrlen, '\0', attrlen);
  return loc;
}
