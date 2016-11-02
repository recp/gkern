/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_common_h
#define gk_common_h

/* since C99 or compiler ext */
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

#if defined(_WIN32)
#  ifdef _gk_dll
#    define GK_EXPORT __declspec(dllexport)
#  else
#    define GK_EXPORT __declspec(dllimport)
#  endif
#  define _gk_hide
#  define GK_INLINE __forceinline
#else
#  define GK_EXPORT   __attribute__((visibility("default")))
#  define _gk_hide    __attribute__((visibility("hidden")))
#  define GK_INLINE inline __attribute((always_inline))
#endif

#define GK_ARRAY_LEN(ARR) sizeof(ARR) / sizeof(ARR[0]);

#endif /* gk_common_h */
