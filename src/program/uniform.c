/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "uniform.h"
#include "uniform_cache.h"

GK_EXPORT
void
gkUniform1i(GkProgram  * __restrict prog,
            const char * __restrict name,
            GLint                   val) {
  glUniform1i(gkUniformLoc(prog, name), val);
}

GK_EXPORT
void
gkUniform1ui(GkProgram  * __restrict prog,
             const char * __restrict name,
             GLuint                  val) {
  glUniform1ui(gkUniformLoc(prog, name), val);
}

GK_EXPORT
void
gkUniform1f(GkProgram  * __restrict prog,
            const char * __restrict name,
            float                   val) {
  glUniform1f(gkUniformLoc(prog, name), val);
}

GK_EXPORT
void
gkUniform2f(GkProgram  * __restrict prog,
            const char * __restrict name,
            float                   val[2]) {
  glUniform2f(gkUniformLoc(prog, name), val[0], val[1]);
}

