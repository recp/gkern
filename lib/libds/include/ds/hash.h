/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef ds_hashtable_h
#define ds_hashtable_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "allocator.h"
#include "util.h"
#include "hash-funcs.h"

#include <stdint.h>

typedef struct HTableItem {
  void              *key;
  void              *data;
  struct HTableItem *next;
} HTableItem;

typedef struct HTable {
  DsAllocator *alc;
  HTableItem **table;
  HashFn       hashfn;
  DsCmpFn      cmp;
  size_t       count;
  uint32_t     capacity;
} HTable;

DS_EXPORT
HTable*
hash_new(DsAllocator *allocator,
         HashFn       hashfn,
         DsCmpFn      cmp,
         uint32_t     capacity);

DS_EXPORT
HTable*
hash_new_str(uint32_t capacity);

DS_EXPORT
HTable*
hash_new_ptr(uint32_t capacity);

DS_EXPORT
HTable*
hash_new_i32(uint32_t capacity);

DS_EXPORT
HTable*
hash_new_ui32(uint32_t capacity);

DS_EXPORT
void
hash_set(HTable *htable,
         void   *key,
         void   *value);

DS_EXPORT
void
hash_unset(HTable *htable,
           void   *key);

DS_EXPORT
void*
hash_get(HTable *htable,
         void   *key);

DS_EXPORT
void
hash_resize(HTable  *htable,
            uint32_t capacity);

DS_EXPORT
void
hash_destroy(HTable *ht);

#ifdef __cplusplus
}
#endif

#endif /* ds_hashtable_h */
