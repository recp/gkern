/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef ds_sort_h
#define ds_sort_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include <stdbool.h>

typedef void (*DsSortSwapFunc)(void * __restrict arr,
                               uint32_t          indexA,
                               uint32_t          indexB);

#define DS_DEFINE_SORT(POSTFIX, TYPE)                                         \
  DS_EXPORT                                                                   \
  void                                                                        \
  sort ## POSTFIX(TYPE * __restrict arr,                                      \
                  uint32_t          count);                                   \
                                                                              \
  DS_EXPORT                                                                   \
  void                                                                        \
  sort ## POSTFIX ## _ex(TYPE * __restrict arr,                               \
                         uint32_t          count,                             \
                         bool              asc,                               \
                         DsSortSwapFunc    swapCb);

/*
 these macros define function declerations.

 for instance DS_DEFINE_SORT(f, float) defines functions for float like below:

 DS_EXPORT
 void
 sortf(float * __restrict arr,
       uint32_t           count);

 DS_EXPORT
 void
 sortf_ex(float * __restrict arr,
          uint32_t           count,
          bool               asc,
          DsSortSwapFunc     swapCb);
 */

DS_DEFINE_SORT(f,   float)
DS_DEFINE_SORT(d,   double)
DS_DEFINE_SORT(i,   int32_t)
DS_DEFINE_SORT(u,   uint32_t)
DS_DEFINE_SORT(i64, int64_t)
DS_DEFINE_SORT(u64, uint64_t)

#undef DS_DEFINE_SORT
  
#endif /* ds_sort_h */
