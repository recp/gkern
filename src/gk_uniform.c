/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_uniform.h"
#include <string.h>

GLint
gkGetUniformLoc(GLuint prog,
                char  * __restrict buf,
                char  * __restrict name) {
  char  *pBuf;
  size_t attrlen;
  GLint  loc;

  if (!buf)
    return glGetUniformLocation(prog, name);

  pBuf = strrchr(buf, '.') + 1;
  strcpy(pBuf, name);

  attrlen = strlen(pBuf);
  loc     = glGetUniformLocation(prog, buf);
  memset((buf + strlen(buf)) - attrlen, '\0', attrlen);
  return loc;
}
