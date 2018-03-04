/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "def_light.h"
#include "../../include/gk/material.h"
#include <stdbool.h>

GkDirectionalLight gkdef_light_directional = {
  .base = {
    .name      = "directional",
    .ref       = {NULL, NULL},
    .next      = NULL,
    .node      = NULL,
    .dir       = {0.0f, 0.0f, -1.0f},
    .defdir    = {0.0f, 0.0f, -1.0f},
    .type      = GK_LIGHT_TYPE_DIRECTIONAL,
    .color     = { 1.0, 1.0, 1.0, 1.0 },
    .index     = -1,
    .isvalid   = false,
    .flags     = 0
  }
};

GkLight*
gk_def_lights() {
  return &gkdef_light_directional.base;
}
