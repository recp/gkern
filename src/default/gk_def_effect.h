/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_def_effect_h
#define gk_def_effect_h

#include "../../include/gk.h"
#include "../../include/gk-material.h"

GkMaterial*
gk_def_material();

GkPhong*
gk_def_material_phong();

GkBlinn*
gk_def_material_blinn();

GkLambert*
gk_def_material_lambert();

GkConstant*
gk_def_material_constant();

#endif /* gk_def_effect_h */
