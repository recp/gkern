/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_transform.h"

GkTransform gkdef_idmat = {
  1,
  GK_TRANSF_LOCAL_ISVALID,
  NULL,
  NULL,
  GLM_MAT4_IDENTITY_INIT,
  GLM_MAT4_IDENTITY_INIT
};


GkTransform *
gk_def_idmat() {
  return &gkdef_idmat;
}
