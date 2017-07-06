/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_def_effect_h
#define gk_def_effect_h

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"

GkMaterial*
gk_def_material(void);

GkPhong*
gk_def_material_phong(void);

GkBlinn*
gk_def_material_blinn(void);

GkLambert*
gk_def_material_lambert(void);

GkConstant*
gk_def_material_constant(void);

#endif /* gk_def_effect_h */
