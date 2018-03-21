/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include <string.h>

#include "../default/def_effect.h"

GkTechnique*
gkMaterialNewPhong() {
  GkTechnique *phong;
  phong = malloc(sizeof(*phong));
  memcpy(phong,
         gk_def_material_phong(),
         sizeof(*phong));
  return phong;
}

GkTechnique*
gkMaterialNewBlinn() {
  GkTechnique *blinn;
  blinn = malloc(sizeof(*blinn));
  memcpy(blinn,
         gk_def_material_blinn(),
         sizeof(*blinn));
  return blinn;
}

GkTechnique*
gkMaterialNewLambert() {
  GkTechnique *lambert;
  lambert = malloc(sizeof(*lambert));
  memcpy(lambert,
         gk_def_material_lambert(),
         sizeof(*lambert));
  return lambert;
}

GkTechnique*
gkMaterialNewConstant() {
  GkTechnique *constant;
  constant = malloc(sizeof(*constant));
  memcpy(constant,
         gk_def_material_constant(),
         sizeof(*constant));
  return constant;
}

GkMetalRough*
gkMaterialNewMetalRough() {
  GkMetalRough *mtlrough;
  mtlrough = malloc(sizeof(*mtlrough));
  memcpy(mtlrough,
         gk_def_material_mtlrough(),
         sizeof(*mtlrough));
  return mtlrough;
}
