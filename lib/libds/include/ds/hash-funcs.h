/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef ds_hash_funcs_h
#define ds_hash_funcs_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include <stdint.h>

typedef uint32_t (*HashFn)(void *key);

DS_EXPORT
uint32_t
ds_hashfn_djb2(void *key);

DS_EXPORT
uint32_t
ds_hashfn_sdbm(void *key);

DS_EXPORT
uint32_t
ds_hashfn_ptr(void *key);

DS_EXPORT
uint32_t
ds_hashfn_ui32(void *key);

DS_EXPORT
uint32_t
ds_hashfn_i32(void *key);

DS_EXPORT
uint32_t
ds_hashfn_i32p(void *key);

DS_EXPORT
uint32_t
ds_hashfn_ui32p(void *key);
  
#ifdef __cplusplus
}
#endif
#endif /* ds_hash_funcs_h */
