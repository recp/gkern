/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "err.h"
#include <assert.h>

void
gkGPUAssertOnError() {
  GLenum err;
  err = glGetError();
  assert(err != GL_INVALID_OPERATION);
}
