/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

 /*
  Types:
    struct FList;
    typedef void (*FListItemFn)(FList *flist, FListItem *item);

  Functions:
    FList* flist_new(DsAllocator *allocator);
    FList* flist_insert(FList *flist, void  *value);
    FList* flist_append(FList *flist, void  *value);
    void   flist_remove(FList *flist, FListItem *item);
    void   flist_remove_by(FList *flist, void *value);
    bool   flist_contains(FList *flist, void *value);
    void   flist_empty(FList *flist);
    bool   flist_isempty(FList *flist);
    void   flist_destroy(FList *flist);
  */

#ifndef ds_forward_list_h
#define ds_forward_list_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "allocator.h"
#include "util.h"
#include "forward-list-common.h"

#include <stdbool.h>

struct FListItem;
struct FList;

typedef void (*FListItemFn)(struct FList *flist, struct FListItem *item);

typedef struct FList {
  FListItem   *first;
  FListItem   *last;
  DsAllocator *alc;
  FListItemFn  onFreeItem;
  size_t       count;
} FList;

/*!
 * @brief creates new forward list
 *
 * @param[in]  allocator custom allocator or NULL for default
 *
 * @return new forward list
 */
DS_EXPORT
FList*
flist_new(DsAllocator *allocator);

/*!
 * @brief inserts new item to first
 *
 * @param[in]  flist forward list
 * @param[in]  value value pointer
 */
DS_EXPORT
void
flist_insert(FList *flist,
             void  *value);

/*!
 * @brief appends new item to last
 *
 * @param[in]  flist forward list
 * @param[in]  value value pointer
 */
DS_EXPORT
void
flist_append(FList *flist,
             void  *value);

/*!
 * @brief index of element (first found element)
 *
 * @param[in]  flist forward list
 * @param[in]  value value pointer
 */
DS_EXPORT
int32_t
flist_indexof(FList *flist,
              void  *value);

/*!
 * @brief remove an item from list by list item, don't free item manually
 *
 * @param[in]  flist forward list
 * @param[in]  item  list item
 */
DS_EXPORT
void
flist_remove(FList *flist, FListItem *item);

/*!
 * @brief remove an item from list by value pointer
 *
 * @param[in]  flist forward list
 * @param[in]  value value pointer
 */
DS_EXPORT
void
flist_remove_by(FList *flist, void *value);

/*!
 * @brief return if list contains value
 *
 * @param[in]  flist forward list
 * @param[in]  value value pointer
 */
DS_EXPORT
bool
flist_contains(FList *flist, void *value);

/*!
 * @brief return item at index
 *
 * @param[in]  flist forward list
 * @param[in]  index index of element
 */
DS_EXPORT
void*
flist_at(FList *flist, int32_t index);

/*!
 * @brief return last item
 *
 * @param[in]  flist forward list
 */
DS_EXPORT
void*
flist_last(FList *flist);
  
/*!
 * @brief remove last item from list without free-ing it
 *
 * @param[in]  flist forward list
 */
DS_EXPORT
void*
flist_pop(FList *flist);

/*!
 * @brief empties forward list, frees all items except list
 *
 * @param[in]  flist forward list
 */
DS_EXPORT
void
flist_empty(FList *flist);

/*!
 * @brief return true if forward list is empty
 *
 * @param[in]  flist forward list
 *
 * @return is empty or not
 */
DS_EXPORT
bool
flist_isempty(FList *flist);

/*!
 * @brief destroys forward list, frees all items
 *
 * @param[in]  flist forward list
 */
DS_EXPORT
void
flist_destroy(FList *flist);

#ifdef __cplusplus
}
#endif
#endif /* ds_forward_list_h */
