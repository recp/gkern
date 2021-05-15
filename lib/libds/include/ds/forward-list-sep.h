/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

/* Forward List vs Forward List Separate?
     Separate version does not keep extra information like allocator, count...
     it reduces FList size. FList groups items with same specific allocator,
     separate version uses default allocator.

     Always pass first element by reference.

     Example:
       typedef struct A_Struct {
          FListItem *values;
       };

       flist_sp_insert(&obj->buffers, value);

       ...

       FListItem *item;
       item = obj->values;
       while (item) {
         value = item->data;
         item  = item->next;
       }
 */

 /*
  Functions:
    void flist_sp_insert(FListItem **first, void *value);
    void flist_sp_append(FListItem **first, void *value);
    void flist_sp_remove(FListItem **first, FListItem *item);
    void flist_sp_remove_by(FListItem **first, void *value);
    bool flist_sp_contains(FListItem **first, void *value);
    void flist_sp_destroy(FListItem **first);
  */

#ifndef ds_forward_list_sep_h
#define ds_forward_list_sep_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "forward-list-common.h"
#include <stdbool.h>

/*!
 * @brief inserts new item to first
 *
 * @param[in]  first pointer to first item
 * @param[in]  value value pointer
 */
DS_EXPORT
void
flist_sp_insert(FListItem **first,
                void       *value);

/*!
 * @brief appends new item to last
 *
 * @param[in]  first pointer to first item
 * @param[in]  value value pointer
 */
DS_EXPORT
void
flist_sp_append(FListItem **first,
                void       *value);

/*!
 * @brief index of element (first found element)
 *
 * @param[in]  first pointer to first item
 * @param[in]  value value pointer
 */
DS_EXPORT
int32_t
flist_sp_indexof(FListItem **first,
                 void       *value);

/*!
 * @brief remove an item from list by list item, don't free item manually
 *
 * @param[in]  first pointer to first item
 * @param[in]  item  list item
 */
DS_EXPORT
void
flist_sp_remove(FListItem **first, FListItem *item);

/*!
 * @brief remove an item from list by value pointer
 *
 * @param[in]  first pointer to first item
 * @param[in]  value value pointer
 */
DS_EXPORT
void
flist_sp_remove_by(FListItem **first, void *value);

/*!
 * @brief return if list contains value
 *
 * @param[in]  first pointer to first item
 * @param[in]  value value pointer
 */
DS_EXPORT
bool
flist_sp_contains(FListItem **first, void *value);

/*!
 * @brief return item at index
 *
 * @param[in]  first pointer to first item
 * @param[in]  index index of element
 */
DS_EXPORT
void*
flist_sp_at(FListItem **first, int32_t index);

/*!
 * @brief return last item
 *
 * @param[in]  first pointer to first item
 */
DS_EXPORT
void*
flist_sp_last(FListItem **first);

/*!
 * @brief destroys forward list, frees all items
 *
 * @param[in]  first pointer to first item
 */
DS_EXPORT
void
flist_sp_destroy(FListItem **first);

#ifdef __cplusplus
}
#endif
#endif /* ds_forward_list_sep_h */
