/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_def_light.h"
#include "../../include/gk-material.h"
#include <stdbool.h>

GkDirectionalLight gkdef_light_directional = {
  .base = {
    .ref       = {NULL, NULL},
    .next      = NULL,
    .node      = NULL,
    .direction = {0.0f, 0.0f, -1.0f},
    .type      = GK_LIGHT_TYPE_DIRECTIONAL,
    .color     = { 1.0, 1.0, 1.0, 1.0 },
    .index     = -1,
    .isvalid   = false,
    .enabled   = true,
    .readonly  = true
  }
};

GkLight*
gk_def_lights() {
  return &gkdef_light_directional.base;
}
