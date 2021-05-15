/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

 /*
  Types:
    struct FListItem;
  */

#ifndef ds_forward_list_common_h
#define ds_forward_list_common_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct FListItem {
  struct FListItem *next;
  void             *data;
} FListItem;

#ifdef __cplusplus
}
#endif
#endif /* ds_forward_list_common_h */
