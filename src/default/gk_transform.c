/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_transform.h"

GkMatrix gkdef_idmat = {
  1,
  1,
  NULL,
  GLM_MAT4_IDENTITY_INIT,
  GLM_MAT4_IDENTITY_INIT
};


GkMatrix *
gk_def_idmat() {
  return &gkdef_idmat;
}
