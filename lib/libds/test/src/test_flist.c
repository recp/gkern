/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "test_common.h"
#include <ds/forward-list.h>

static
void
test_flist_onfree(FList *flist, FListItem *item) {
  assert(item);
  assert(item->data);

  /* free value */
  free(item->data);
}

TEST_IMPL(flist) {
  FList *flist;
  float *value;
  int    count, i;

  count = 200;
  flist = flist_new(NULL);
  flist->onFreeItem = test_flist_onfree;

  srand((unsigned int)time(NULL));

  for (i = 0; i < count; i++) {
    value  = malloc(sizeof(*value));
    *value = (float)drand48();

    /* test insert */
    if (i % 2 == 0) {
      flist_insert(flist, value);
      ASSERT(flist->first);
      ASSERT(flist->last);
    }

    /* test append */
    else {
      flist_append(flist, value);
      ASSERT(flist->first);
      ASSERT(flist->last);
    }

    /* test find node */
    ASSERT(flist_contains(flist, value));

    /* test remove */
    if (i == 0) {
      flist_remove_by(flist, value);
      ASSERT(!flist_contains(flist, value));
    }

    /* test remove by item */
    if (i == 10) {
      FListItem *item;
      float     *val;

      ASSERT(flist_last(flist) == flist_at(flist, 9));
      
      item = flist->first;
      val  = item->data;
      flist_remove(flist, item);
      ASSERT(!flist_contains(flist, val));
    }

    if (i == 20 && flist->last) {
      FListItem *item;
      float     *val;

      item = flist->last;
      val  = item->data;

      flist_remove(flist, item);

      if (flist->first)
        ASSERT(flist->last);

      ASSERT(!flist_contains(flist, val));
    }

    if (i == 30) {
      void *val;
      val = flist->last->data;

      flist_remove_by(flist, val);
      ASSERT(!flist_contains(flist, val));
    }

    /* pick random item */
    if (i == 40) {
      FListItem *item1, *item2;
      float     *val1,  *val2;
      int        i;

      item1 = flist->first;
      for (i = 0; i < 5; i++) {
        item1 = item1->next;
      }

      item2 = item1->next->next;

      val1  = item1->data;
      val2  = item2->data;
      flist_remove(flist, item1);
      flist_remove(flist, item2);

      ASSERT(!flist_contains(flist, val1));
      ASSERT(!flist_contains(flist, val2));
    }

    /* test empty */
    if (i == 100) {
      flist_empty(flist);
      ASSERT(flist_isempty(flist));
    }
  }

  flist_destroy(flist);

  TEST_SUCCESS
}
