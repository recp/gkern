/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "common.h"
#include "../include/ds/allocator.h"

DsAllocator *ds__custom_alc;

DS_EXPORT
void
ds_set_allocator(DsAllocator *alc) {
  ds__custom_alc = alc;
}

DS_EXPORT
DsAllocator*
ds_allocator(void) {
  return !ds__custom_alc ? ds_def_alc() : ds__custom_alc;
}
