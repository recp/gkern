/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#include "animatable.h"

GkAnimatable*
gkAnimatable(void *object) {
  GkAnimatable *anima;
  anima = malloc(sizeof(*anima));
  anima->animations = flist_new(NULL);
  return anima;
}

GK_EXPORT
void
gkAddAnimation(GkScene     *scene,
               GkNode      *node,
               GkAnimation *anim) {
  GkSceneImpl *sceneImpl;

  sceneImpl   = (GkSceneImpl *)scene;
  anim->scene = scene;

  if (node) {
    anim->node = node;
    flist_insert(node->anim->animations, anim);
  }

  flist_sp_insert(&sceneImpl->anims, anim);
}
