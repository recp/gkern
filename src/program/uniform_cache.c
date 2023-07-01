/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../common.h"
#include "uniform_cache.h"
#include <string.h>

GK_EXPORT
GLint
gkUniformLoc(GkPipeline *prog, const char *name) {
  void *found;
  GLint loc;

  if (!prog->uniforms)
    prog->uniforms = hash_new_str(8);

  if ((found = hash_get(prog->uniforms, (void *)name)))
    return ((GLint)(uintptr_t)found) - 1;

  loc = glGetUniformLocation(prog->progId, name);
  hash_set(prog->uniforms,
           (void *)strdup(name),
           (void *)(uintptr_t)(loc + 1));

  return loc;
}

GLint
gkUniformLocBuff(GkPipeline * __restrict prog,
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
