/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_options_h
#define gk_options_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef enum GkOption {
  GK_OPT_LIGHT_DIR           = 0,  /* 0, 0, -1    */
  GK_OPT_LIGHT_UP            = 1   /* 0, 1,  0    */
} GkOption;

GK_EXPORT
void
gk_opt_set(GkOption option, uintptr_t value);

GK_EXPORT
uintptr_t
gk_opt(GkOption option);

#ifdef __cplusplus
}
#endif
#endif /* gk_options_h */

