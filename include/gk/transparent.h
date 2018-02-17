/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef transparency_h
#define transparency_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

struct GkScene;

/* Transparent technique type */

typedef enum GkTranspTechnType {
  GK_TRANSP_AUTO             = 0,
  GK_TRANSP_DISABLED         = 1,
  GK_TRANSP_ALPHA_BLENDED    = 2, /* ordering is required */
  GK_TRANSP_WEIGHTED_BLENDED = 3,
  GK_TRANSP_PHENOMENOLOGICAL = 4,

  GK_TRANSP_DEFAULT          = GK_TRANSP_WEIGHTED_BLENDED
} GkTranspTechnType;

GK_EXPORT
void
gkSetTranspTechn(GkTranspTechnType techn);

GK_EXPORT
GkTranspTechnType
gkTranspTechn(void);

GK_EXPORT
void
gkEnableTransp(GkScene * __restrict scene);

GK_EXPORT
void
gkDisableTransp(GkScene * __restrict scene);

#ifdef __cplusplus
}
#endif
#endif /* transparency_h */
