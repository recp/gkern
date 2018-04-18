/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_trackball_h
#define gk_trackball_h

#include "common.h"
#include "geom-types.h"
#include "mouse.h"

#include <cglm/cglm.h>
#include <stdbool.h>

struct GkScene;
struct GkTrackball;
struct GkNode;

typedef enum GkTrackballEventType {
  GK_TRACKBALL_EVENT_BEGIN = 0,
  GK_TRACKBALL_EVENT_END   = 1
} GkTrackballEventType;

typedef void (*GkTrackballEventCb)(struct GkTrackball *tball,
                                   GkTrackballEventType event);

typedef struct GkTrackball {
  void                  *data;
  struct GkScene        *scene;
  struct GkNode         *node;
  vec3                  *bbox;      /* attached node's bounding box  */
  GkTransform           *nodeTrans; /* attached node's transform     */
  GkMouseEvent           mouse;
  GkMouseEventWithStruct mouse2;
  GkTrackballEventCb     cb;
  GkPoint                start;
  vec3                   from;
  vec3                   center;
  mat4                   trans;
  bool                   enabled;
  bool                   moving;
  float                  velocity;
} GkTrackball;

GkTrackball*
gkTrackballAlloc(void);

void
gkTrackballAttach(GkTrackball * __restrict tball,
                  GkScene     * __restrict scene,
                  GkNode      * __restrict node,
                  GkBBox                   bbox);

void
gkTrackballFree(GkTrackball *tball);

#endif /* gk_trackball_h */
