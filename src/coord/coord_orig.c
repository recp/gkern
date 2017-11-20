/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/coord.h"

void
gk_moveOrigin(vec3 position,
              vec3 orig,
              vec3 out) {
  glm_vec_sub(orig, position, out);
}
