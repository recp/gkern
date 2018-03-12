/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "err.h"
#include <assert.h>

/* TODO: log and raise warning/errors */

void
gkGPUAssertOnError() {
  GLenum err;
  err = glGetError();
  assert(err != GL_INVALID_OPERATION);
}

void
gkGPUFrameBuffAssertOnError() {
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}
