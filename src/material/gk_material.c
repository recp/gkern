/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include <string.h>

#include "../gk_uniform.h"
#include "../default/gk_def_effect.h"

GkPhong*
gkMaterialNewPhong() {
  GkPhong *phong;
  phong = malloc(sizeof(*phong));
  memcpy(phong,
         gk_def_material_phong(),
         sizeof(*phong));
  return phong;
}

GkBlinn*
gkMaterialNewBlinn() {
  GkBlinn *blinn;
  blinn = malloc(sizeof(*blinn));
  memcpy(blinn,
         gk_def_material_blinn(),
         sizeof(*blinn));
  return blinn;
}

GkLambert*
gkMaterialNewLambert() {
  GkLambert *lambert;
  lambert = malloc(sizeof(*lambert));
  memcpy(lambert,
         gk_def_material_lambert(),
         sizeof(*lambert));
  return lambert;
}

GkConstant*
gkMaterialNewConstant() {
  GkConstant *constant;
  constant = malloc(sizeof(*constant));
  memcpy(constant,
         gk_def_material_constant(),
         sizeof(*constant));
  return constant;
}
