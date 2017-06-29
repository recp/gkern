/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_model.h"
#include "gk_common.h"
#include "time/gk_time.h"

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
}

void
GK_DESTRUCTOR
gk__cleanup() {
  gk_time_deinit();
  gk_verta_deinit();
}
