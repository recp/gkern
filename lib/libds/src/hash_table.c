/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "common.h"
#include "../include/ds/hash.h"

#include <assert.h>
#include <stdbool.h>

static
bool
hash_finditem(HTable      *htable,
              void        *key,
              uint32_t    *idx_out,
              HTableItem **found,
              HTableItem **prev);

static
void
hash_insert(HTable     *htable,
            HTableItem *item);

DS_EXPORT
HTable*
hash_new(DsAllocator *allocator,
         HashFn       hashfn,
         DsCmpFn      cmp,
         uint32_t     capacity) {
  DsAllocator *alc;
  HTable   *htable;

  alc      = !allocator ? ds_allocator() : allocator;
  htable   = alc->calloc(1, sizeof(*htable));

  capacity = (uint32_t)ds_prime_num(capacity);
  htable->table = alc->calloc(capacity, sizeof(*htable->table));

  assert(htable);

  htable->alc      = alc;
  htable->hashfn   = hashfn;
  htable->cmp      = cmp ? cmp : ds_cmp_str;
  htable->count    = 0;
  htable->capacity = capacity;

  return htable;
}

DS_EXPORT
HTable*
hash_new_str(uint32_t capacity) {
  return hash_new(ds_allocator(),
                  ds_hashfn_djb2,
                  ds_cmp_str,
                  capacity);
}

DS_EXPORT
HTable*
hash_new_ptr(uint32_t capacity) {
  return hash_new(ds_allocator(),
                  ds_hashfn_ptr,
                  ds_cmp_ptr,
                  capacity);
}

DS_EXPORT
HTable*
hash_new_i32(uint32_t capacity) {
  return hash_new(ds_allocator(),
                  ds_hashfn_i32,
                  ds_cmp_i32,
                  capacity);
}

DS_EXPORT
HTable*
hash_new_ui32(uint32_t capacity) {
  return hash_new(ds_allocator(),
                  ds_hashfn_ui32,
                  ds_cmp_ui32,
                  capacity);
}

static
bool
hash_finditem(HTable      *htable,
              void        *key,
              uint32_t    *idx_out,
              HTableItem **found,
              HTableItem **prev) {
  HTableItem *item;
  uint32_t       idx;

  *prev    = NULL;
  *idx_out = idx = htable->hashfn(key) % htable->capacity;

  if ((item = htable->table[idx])) {
    do {
      *found = item;

      if (htable->cmp(key, item->key) == 0)
        return true;

      *prev = item;
    } while ((item = item->next));
  }

  *found = NULL;
  return false;
}

DS_EXPORT
void
hash_unset(HTable *htable,
           void      *key) {
  HTableItem *found, *item, *prev;
  uint32_t    idx;

  if (!hash_finditem(htable, key, &idx, &found, &prev) || !found)
    return;

  item = htable->table[idx];
  if (found == item)
    htable->table[idx] = found->next;

  if (prev)
    prev->next = found->next;

  htable->alc->free(found);
  htable->count--;
}

static
void
hash_insert(HTable     *htable,
            HTableItem *item) {
  HTableItem *prev, *found;
  uint32_t    idx;

  /* replace */
  if (hash_finditem(htable, item->key, &idx, &found, &prev)) {
    hash_unset(htable, item->key);
    hash_insert(htable, item);
    return;
  }

  if (!prev) {
    item->next         = htable->table[idx];
    htable->table[idx] = item;
  }

  /* there is no need to access buffer (read + write),
   just append it */
  else {
    prev->next = item;
    item->next = NULL;
  }

  htable->count++;
}

DS_EXPORT
void
hash_set(HTable *htable,
         void   *key,
         void   *value) {
  HTableItem *prev, *found, *item;
  uint32_t    idx;

  if (!value) {
    hash_unset(htable, key);
    return;
  }

  if (hash_finditem(htable, key, &idx, &found, &prev)) {
    found->key  = key;
    found->data = value;
    return;
  }

  item = htable->alc->calloc(1, sizeof(*item));
  item->key          = key;
  item->data         = value;

  if (!prev) {
    item->next         = htable->table[idx];
    htable->table[idx] = item;
  }

  /* there is no need to access buffer (read + write),
     just append it */
  else {
    prev->next = item;
  }

  htable->count++;
}

DS_EXPORT
void*
hash_get(HTable *htable,
         void   *key) {
  HTableItem *prev, *found;
  uint32_t    idx;

  if (!hash_finditem(htable, key, &idx, &found, &prev) || !found)
    return NULL;

  return found->data;
}

DS_EXPORT
void
hash_resize(HTable  *htable,
            uint32_t capacity) {
  DsAllocator *alc;
  HTableItem **table;
  HTableItem  *first, *last,  *item;
  uint32_t     i;

  capacity = (uint32_t)ds_prime_num(capacity);
  if (htable->capacity == capacity)
    return;

  first = NULL;
  table = htable->table;

  /* create linear list */
  for (i = 0; i < htable->capacity; i++) {
    if (!(item = table[i]))
      continue;

    last = item;
    while (last->next)
      last = last->next;

    last->next = first;
    first      = item;
  }

  alc = htable->alc;

  /* free / malloc */
  if (capacity > htable->capacity) {
    alc->free(table);
    table = alc->calloc(capacity, sizeof(*table));
  }

  /* realloc must be faster than free/malloc here */
  else {
    size_t newsize;
    newsize = sizeof(*table) * capacity;
    table   = alc->realloc(table, newsize);
    memset(table, '\0', newsize);
  }

  htable->table    = table;
  htable->count    = 0;
  htable->capacity = capacity;

  /* re-insert all items to resized table */
  while (first) {
    item  = first;
    first = first->next;
    hash_insert(htable, item);
  }
}

DS_EXPORT
void
hash_destroy(HTable *ht) {
  DsAllocator *alc;
  HTableItem **table;
  HTableItem  *item, *tofree;
  uint32_t     i;

  alc   = ht->alc;
  table = ht->table;

  for (i = 0; i < ht->capacity; i++) {
    if (!(item = table[i]))
      continue;

    while (item) {
      tofree = item;
      item = item->next;
      alc->free(tofree);
    }
  }

  alc->free(table);
  alc->free(ht);
}
