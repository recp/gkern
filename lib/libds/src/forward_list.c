/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "common.h"
#include "../include/ds/forward-list.h"
#include "../include/ds/forward-list-sep.h"

#include <assert.h>

static
void
flist_perform_rm(FList     *flist,
                 FListItem *prev,
                 FListItem *tofree);
DS_EXPORT
FList*
flist_new(DsAllocator *allocator) {
  DsAllocator *alc;
  FList       *flist;

  alc   = !allocator ? ds_allocator() : allocator;
  flist = alc->calloc(1, sizeof(*flist));

  assert(flist);

  flist->alc   = alc;
  flist->count = 0;

  return flist;
}

DS_EXPORT
void
flist_insert(FList *flist,
             void  *value) {
  FListItem *item;

  item = flist->alc->calloc(1, sizeof(*item));
  item->data = value;

  item->next = flist->first;
  if (!flist->last) {
#if DEBUG
    assert(!flist->first && flist->count == 0);
#endif
    flist->last = item;
  }

  flist->first = item;
  flist->count++;
}

DS_EXPORT
void
flist_append(FList *flist,
             void  *value) {
  FListItem *item;

  item = flist->alc->calloc(1, sizeof(*item));
  item->data = value;
  item->next = NULL; /* we are appending it */

  if (flist->last) {
#if DEBUG
    assert(flist->first && flist->count > 0);
#endif
    flist->last->next = item;
  }

  /* if last == NULL then first == NULL too */
  else {
#if DEBUG
    assert(!flist->first && flist->count == 0);
#endif
    flist->first = item;
  }

  flist->last = item;
  flist->count++;
}

DS_EXPORT
int32_t
flist_indexof(FList *flist,
              void  *value) {
  return flist_sp_indexof(&flist->first, value);
}

static
void
flist_perform_rm(FList     *flist,
                 FListItem *prev,
                 FListItem *tofree) {
  /* fire events */
  if (flist->onFreeItem)
    flist->onFreeItem(flist, tofree);

  /* special case; ingle item */
  if (flist->first == tofree
      && flist->last == tofree) {
    flist->first = flist->last = NULL;
#if DEBUG
    assert(flist->count == 1);
#endif
    flist->count = 0;
    flist->alc->free(tofree);
    return;
  }

  if (flist->first == tofree) {
    flist->first = tofree->next;
#if DEBUG
    assert(flist->first);
#endif
  }

  if (flist->last == tofree) {
    flist->last = prev;
#if DEBUG
    assert(flist->last);
#endif
  }

  if (prev->next == tofree)
    prev->next = tofree->next;

  flist->alc->free(tofree);
  flist->count--;
}

DS_EXPORT
void
flist_remove(FList *flist, FListItem *item) {
  FListItem *prev, *tofree;

  prev = flist->first;
  if (!prev)
    return;

  if (prev == item) {
    tofree = prev;
    goto freeitm;
  }

  tofree = NULL;
  while (prev->next) {
    if (prev->next == item) {
      tofree = prev->next;
      break;
    }
    prev = prev->next;
  }

  if (!tofree)
    return;

freeitm:
  flist_perform_rm(flist, prev, tofree);
}

DS_EXPORT
void
flist_remove_by(FList *flist, void *value) {
  FListItem *prev, *tofree;

  prev = flist->first;
  if (!prev)
    return;

  if (prev->data == value) {
    tofree = prev;
    goto freeitm;
  }

  tofree = NULL;
  while (prev->next) {
    if (prev->next->data == value) {
      tofree = prev->next;
      break;
    }
    prev = prev->next;
  }

  if (!tofree)
    return;

freeitm:
  flist_perform_rm(flist, prev, tofree);
}

DS_EXPORT
bool
flist_contains(FList *flist, void *value) {
  FListItem *item;

  item = flist->first;
  while (item) {
    if (item->data == value)
      return true;
    item = item->next;
  }

  return false;
}

DS_EXPORT
void*
flist_at(FList *flist, int32_t index) {
  return flist_sp_at(&flist->first, index);
}

DS_EXPORT
void*
flist_last(FList *flist) {
  FListItem *item;

  if ((item = flist->last))
    return item->data;

  return NULL;
}

DS_EXPORT
void*
flist_pop(FList *flist) {
  FListItem *item, *prev, *tofree;
  void      *data;

  if (!(item = flist->last))
    return NULL;

  data = item->data;
  prev = flist->first;
  if (!prev)
    return data;

  if (prev == item) {
    tofree = prev;
    goto freeitm;
  }

  tofree = NULL;
  while (prev->next) {
    if (prev->next == item) {
      tofree = prev->next;
      break;
    }
    prev = prev->next;
  }

  if (!tofree)
    return data;

freeitm:
  flist_perform_rm(flist, prev, tofree);
  return data;
}

DS_EXPORT
void
flist_empty(FList *flist) {
  DsAllocator *alc;
  FListItem   *item, *tofree;

  alc  = flist->alc;
  item = flist->first;

  tofree = NULL;
  while (item) {
    tofree = item;
    item   = item->next;

    if (flist->onFreeItem)
      flist->onFreeItem(flist, tofree);

    alc->free(tofree);
  }

  flist->first = NULL;
  flist->last  = NULL;
  flist->count = 0;
}

DS_EXPORT
bool
flist_isempty(FList *flist) {
  return flist->first == NULL
          && flist->last == NULL;
}

DS_EXPORT
void
flist_destroy(FList *flist) {
  flist_empty(flist);
  flist->alc->free(flist);
}
