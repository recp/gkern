/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "test_common.h"
#include <ds/hash.h>
#include <ds/util.h>

/* keep top 2000 to test remove */
static char *inserted_items[2000];

TEST_IMPL(htable) {
  char     keybuf[256];
  HTable  *htable;
  void    *found;
  char    *key;
  uint32_t count, i, k;
  uint32_t capacity;

  capacity = ds_prime_num(3);
  htable   = hash_new_str(capacity);

  /* 25 is not a prime number */
  ASSERT(htable->capacity > 3);

  count = 1000;

  srand((unsigned int)time(NULL));

  for (i = 0; i < 2; i++) {
    /* random key length */
    k = rand() % (sizeof(keybuf) / 8 - 1);

    rand_str(keybuf, k);
    key = strdup(keybuf);

    hash_set(htable, key, key);

    /* test find value */
    found = hash_get(htable, key);
    ASSERT(found);

    /* found values must be same */
    ASSERT(key == found);
    inserted_items[i] = key;
  }

  /* increase size */
  hash_resize(htable, 9);
  ASSERT(htable->count == 2);

  for (i = 0; i < count; i++) {
    /* random key length */
    k = rand() % (sizeof(keybuf) / 8 - 1);

    rand_str(keybuf, k);
    key = strdup(keybuf);

    hash_set(htable, key, key);

    /* test find value */
    found = hash_get(htable, key);
    ASSERT(found);

    /* found values must be same */
    ASSERT(key == found);
    inserted_items[i] = key;

    if (i == 0 || i == 10 || i == 50) {
      hash_unset(htable, key);

      /* try to unset again */
      hash_unset(htable, key);

      found = hash_get(htable, key);
      ASSERT(!found);

      /* allow only once */
      hash_set(htable, key, key);
      hash_set(htable, key, key);
      hash_set(htable, key, key);

      /* test remove by NULL */
      hash_set(htable, key, NULL);

      found = hash_get(htable, key);
      ASSERT(!found);
    }
  }

  count = (uint32_t)htable->count;

  /* test resize */

  /* increase size */
  hash_resize(htable, 100);
  ASSERT(htable->count == count);

  /* decrease size */
  hash_resize(htable, 15);
  ASSERT(htable->count == count);

  for (i = 0; i < 10; i++) {
    /* random key length */
    k = rand() % (sizeof(keybuf) / 8 - 1);

    rand_str(keybuf, k);
    key = strdup(keybuf);

    hash_set(htable, key, key);

    /* test find value */
    found = hash_get(htable, key);
    ASSERT(found);

    /* found values must be same */
    ASSERT(key == found);
    inserted_items[i] = key;

    if (i == 10 || i == 50) {
      hash_unset(htable, key);
      found = hash_get(htable, key);
      ASSERT(!found);
    }
  }

  count = (uint32_t)htable->count;

  /* test other hash funcs */
  htable->hashfn = ds_hashfn_sdbm;

  hash_resize(htable, 50);
  ASSERT(htable->count == count);

  TEST_SUCCESS
}
