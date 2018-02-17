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
#  define likely(x)   x
#  define unlikely(x) x
#else
#  define GK_EXPORT   __attribute__((visibility("default")))
#  define _gk_hide    __attribute__((visibility("hidden")))
#  define GK_INLINE inline __attribute((always_inline))
#  define likely(x)      __builtin_expect(!!(x), 1)
#  define unlikely(x)    __builtin_expect(!!(x), 0)
#endif

#define GK_ARRAY_LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

#define GK_STRINGIFY(...)  #__VA_ARGS__
#define GK_STRINGIFY2(x)   GK_STRINGIFY(x)

#define GK_FLG(FLAGS, FLAG) ((FLAGS & FLAG) == FLAG)

#ifdef __cplusplus
#define GK_MAKE_C_ENUM(ENM)                                                    \
static inline ENM operator|=(ENM& a, ENM b) {                                         \
  return (a = static_cast<ENM>(static_cast<int>(a) | static_cast<int>(b)));    \
}                                                                              \
                                                                               \
static inline ENM operator&=(ENM& a, ENM b) {                                         \
  return (a = static_cast<ENM>(static_cast<int>(a) & static_cast<int>(b)));    \
}                                                                              \
                                                                               \
static inline ENM operator~(ENM a) {                                                  \
  return static_cast<ENM>(~static_cast<int>(a));                               \
}                                                                              \
                                                                               \
static inline ENM operator|(ENM a, ENM b) {                                           \
  return static_cast<ENM>(static_cast<int>(a) | static_cast<int>(b));          \
}                                                                              \
                                                                               \
static inline ENM operator&(ENM a, ENM b) {                                           \
  return static_cast<ENM>(static_cast<int>(a) & static_cast<int>(b));          \
}

#else
#define GK_MAKE_C_ENUM(ENM) 
#endif

#include <stdint.h>
#include <stdbool.h>

#ifdef __APPLE__
#  include <OpenGL/gl3.h>
#  include <OpenGL/gl3ext.h>
#endif

#endif /* gk_common_h */
