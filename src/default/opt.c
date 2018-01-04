/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/opt.h"

vec3 gk__light_dir = {0.0f, 0.0f, -1.0f};
vec3 gk__light_up  = {0.0f, 1.0f,  0.0f};

uintptr_t GK_OPTIONS[] =
{
  (uintptr_t)&gk__light_dir,
  (uintptr_t)&gk__light_up
};

GK_EXPORT
void
gk_opt_set(GkOption option, uintptr_t value) {
  GK_OPTIONS[option] = value;
}

GK_EXPORT
uintptr_t
gk_opt(GkOption option) {
  return GK_OPTIONS[option];
}
