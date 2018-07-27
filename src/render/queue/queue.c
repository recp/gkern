

GkRenderItem*
gkAllocRenderQueue(int32_t count) {
  GkRenderItem* itemList;

  itemList = malloc(sizeof(*itemList) * count);
  return itemList;
}


void
gkLinearizeScene(GkScene * __restrict scene) {
  
}
