/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_platform_h
#define gk_platform_h
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* this caches common queries */
typedef enum GkPlatformInfo {
  GK_PLI_MAX_TEX_UNITS = 0
} GkPlatformInfo;

GLint
gkPlatfomInfo(GkPlatformInfo pi);

void
gkSetGLContext(void *glcontext);

#ifdef __cplusplus
}
#endif
#endif /* gk_platform_h */
