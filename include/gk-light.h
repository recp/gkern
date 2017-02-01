/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_light_h
#define gk_light_h

#include <cglm.h>

struct GkScene;
struct GkNode;

typedef enum GkLightType {
  GK_LIGHT_TYPE_AMBIENT     = 1,
  GK_LIGHT_TYPE_DIRECTIONAL = 2,
  GK_LIGHT_TYPE_POINT       = 3,
  GK_LIGHT_TYPE_SPOT        = 4,
  GK_LIGHT_TYPE_CUSTOM      = 5
} GkLightType;

/* for scene, because node can have multiple light instancs */
typedef struct GkLightRef {
  struct GkLightRef *prev;
  struct GkLightRef *next;
} GkLightRef;

struct GkNode;
typedef struct GkLight {
  GkLightRef       ref;
  struct GkLight  *next;
  struct GkNode   *node;
  vec3             direction;
  GkLightType      type;
  GkColor          color;
  GLint            index;
  uint8_t          isvalid;
  uint8_t          enabled;
} GkLight;

typedef GkLight GkAmbientLight;

typedef struct GkDirectionalLight {
  GkLight base;
} GkDirectionalLight;

typedef struct GkPointLight {
  GkLight base;
  float   constAttn;
  float   linearAttn;
  float   quadAttn;
} GkPointLight;

typedef struct GkSpotLight {
  GkLight base;
  float   constAttn;
  float   linearAttn;
  float   quadAttn;
  float   cutoffCosine;
  float   cutoffExp;
} GkSpotLight;

void
gkUniformLights(struct GkScene * __restrict scene);

void
gkUniformLight(struct GkScene * __restrict scene,
               GkLight        * __restrict light);

#endif /* gk_light_h */
