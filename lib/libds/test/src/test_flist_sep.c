/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "test_common.h"
#include <ds/forward-list-sep.h>
#include <time.h>

TEST_IMPL(flist_sep) {
  FListItem *first;
  float     *value;
  int       count, i;

  count = 200;
  first = NULL;

  srand((unsigned int)time(NULL));

  for (i = 0; i < count; i++) {
    value  = malloc(sizeof(*value));
    *value = (float)drand48();

    /* test insert */
    if (i % 2 == 0) {
      flist_sp_insert(&first, value);
      ASSERT(first);
    }

    /* test append */
    else {
      flist_sp_append(&first, value);
      ASSERT(first);
    }

    /* test find node */
    ASSERT(flist_sp_contains(&first, value));

    /* test remove */
    if (i == 0) {
      flist_sp_remove_by(&first, value);
      ASSERT(!flist_sp_contains(&first, value));
    }

    /* test remove by item */
    if (i == 10) {
      FListItem *item;
      float     *val;

      ASSERT(flist_sp_last(&first) == flist_sp_at(&first, 9));
      
      item = first;
      val  = item->data;
      flist_sp_remove(&first, item);
      ASSERT(!flist_sp_contains(&first, val));
    }

    if (i == 30) {
      void *val;
      val = first->data;
      flist_sp_remove_by(&first, val);
      ASSERT(!flist_sp_contains(&first, val));
    }

    /* pick random item */
    if (i == 40) {
      FListItem *item1, *item2;
      float     *val1,  *val2;
      int        i;

      item1 = first;
      for (i = 0; i < 5; i++) {
        item1 = item1->next;
      }

      item2 = item1->next->next;

      val1  = item1->data;
      val2  = item2->data;
      flist_sp_remove(&first, item1);
      flist_sp_remove(&first, item2);

      ASSERT(!flist_sp_contains(&first, val1));
      ASSERT(!flist_sp_contains(&first, val2));
    }

    /* test empty */
    if (i == 100) {
      flist_sp_destroy(&first);
      ASSERT(!first);
    }
  }

  flist_sp_destroy(&first);

  TEST_SUCCESS
}

