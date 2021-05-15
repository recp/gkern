/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "common.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

DS_EXPORT
int
ds_cmp_str(void * key1, void *key2) {
  return strcmp((char *)key1, (char *)key2);
}

DS_EXPORT
int
ds_cmp_ptr(void *key1, void *key2) {
  if (key1 > key2)
    return 1;
  else if (key1 < key2)
    return -1;
  return 0;
}

DS_EXPORT
int
ds_cmp_i32(void *key1, void *key2) {
  int32_t a, b;

  a = *(int32_t *)key1;
  b = *(int32_t *)key2;

  return a - b;
}

DS_EXPORT
int
ds_cmp_ui32(void *key1, void *key2) {
  uint32_t a, b;
  
  a = *(uint32_t *)key1;
  b = *(uint32_t *)key2;
  
  return a - b;
}

DS_EXPORT
int
ds_cmp_i32p(void *key1, void *key2) {
  int32_t a, b;

  a = (int32_t)(intptr_t)key1;
  b = (int32_t)(intptr_t)key2;

  return a - b;
}

DS_EXPORT
int
ds_cmp_ui32p(void *key1, void *key2) {
  uint32_t a, b;

  a = (uint32_t)(uintptr_t)key1;
  b = (uint32_t)(uintptr_t)key2;

  return a - b;
}

DS_EXPORT
int
ds_cmp_i64(void *key1, void *key2) {
  int64_t a, b;

  a = *(int64_t *)key1;
  b = *(int64_t *)key2;

  return (int)(a - b);
}

DS_EXPORT
int
ds_cmp_ui64(void *key1, void *key2) {
  uint64_t a, b;

  a = *(uint64_t *)key1;
  b = *(uint64_t *)key2;

  return (int)(a - b);
}

DS_EXPORT
void
ds_print_str(void *key) {
  printf("\t'%s'\n", (const char *)key);
}

DS_EXPORT
void
ds_print_ptr(void *key) {
  printf("\t'%p'\n", key);
}

DS_EXPORT
void
ds_print_i32(void *key) {
  printf("\t%d\n", *(int32_t *)key);
}

DS_EXPORT
void
ds_print_ui32(void *key) {
  printf("\t%u\n", *(uint32_t *)key);
}

DS_EXPORT
void
ds_print_i64(void *key) {
  printf("\t%llu\n", *(int64_t *)key);
}

DS_EXPORT
void
ds_print_ui64(void *key) {
  printf("\t%llu\n", *(uint64_t *)key);
}

DS_EXPORT
int32_t
ds_prime_num(int32_t min) {
  int32_t i;

  /* find closest prime */
again:
  for(min++, i = 2; i <= min / 2; ++i) {
    if(min % i == 0)
      goto again;
  }

  return min;
}
