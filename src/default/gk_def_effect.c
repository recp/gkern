/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_def_effect.h"

GkPhong gkdef_phong = {
  .base = {
    .type       = GK_MATERIAL_PHONG,
    .subroutine = "phong"
  },
  .emission    = { .color = { 0.0, 0.0, 0.0, 1.0 } },
  .ambient     = { .color = { 1.0, 1.0, 1.0, 1.0 } },
  .diffuse     = { .color = { 1.0, 1.0, 1.0, 1.0 } },
  .specular    = { .color = { 1.0, 1.0, 1.0, 1.0 } },
  .reflective  = { .color = { 1.0, 1.0, 1.0, 1.0 } },
  .transparent = { .color = { 0.0, 0.0, 0.0, 1.0 } },
  .shininess         = 1.0,
  .reflectivity      = 1.0,
  .transparency      = 0.0,
  .indexOfRefraction = 1.0
};

GkMaterial gkdef_material = {
  .technique = &gkdef_phong.base,
  .isvalid   = 1,
  .enabled   = 1
};

GkMaterial*
gk_def_material() {
  return &gkdef_material;
}
