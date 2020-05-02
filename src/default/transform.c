/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
