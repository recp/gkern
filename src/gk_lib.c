/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_model.h"
#include "gk_common.h"

void
gk_verta_init();

void
gk_verta_deinit();

void
GK_CONSTRUCTOR
gk__init() {
  gk_verta_init();
}

void
GK_DESTRUCTOR
gk__cleanup() {
  gk_verta_deinit();
}
