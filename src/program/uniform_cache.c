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

GLint
gkUniformLocBuff(GkProgInfo * __restrict pinfo,
                 char       * __restrict name,
                 char       * __restrict buf) {
  char  *pBuf;
  size_t attrlen;
  GLint  loc;
  
  if (!buf)
    return gkUniformLoc(pinfo, name);
  
  pBuf = strrchr(buf, '.') + 1;
  strcpy(pBuf, name);
  
  attrlen = strlen(pBuf);
  loc     = gkUniformLoc(pinfo, buf);
  memset((buf + strlen(buf)) - attrlen, '\0', attrlen);
  return loc;
}
