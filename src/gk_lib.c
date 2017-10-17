/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_model.h"
#include "gk_common.h"
#include "time/gk_time.h"

#include "shader/shader.h"
#include "program/program.h"

void
gk_verta_init(void);

void
gk_verta_deinit(void);

void
GK_CONSTRUCTOR
gk__init(void);

void
GK_DESTRUCTOR
gk__cleanup(void);

void
GK_CONSTRUCTOR
gk__init() {
  gk_verta_init();
  gk_time_init();
  gk_shaders_init();
  gk_prog_init();
}

void
GK_DESTRUCTOR
gk__cleanup() {
  gk_prog_deinit();
  gk_shaders_deinit();
  gk_time_deinit();
  gk_verta_deinit();
}
