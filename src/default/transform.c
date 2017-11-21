/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "transform.h"
#include "../types/impl_transform.h"

GkTransformImpl gkdef_idmat = {
  .pub = {
    GLM_MAT4_IDENTITY_INIT,
    GLM_MAT4_IDENTITY_INIT,
    GK_TRANSF_LOCAL_ISVALID,
    NULL
  },
  0,
  0,
  NULL
};

GkTransform *
gk_def_idmat() {
  return &gkdef_idmat.pub;
}
