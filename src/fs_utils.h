/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef fs_utils_h
#define fs_utils_h

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int
fsReadfile(const char * __restrict file,
           const char * __restrict modes,
           char ** __restrict dest);

#ifdef __cplusplus
}

#endif

#endif /* fs_utils_h */
