/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_light_h
#define gk_light_h

#include "common.h"
#include "color.h"
#include <cglm/cglm.h>

struct GkScene;
struct GkNode;
struct GkProgram;
struct GkColorOrTex;
struct GkTransform;

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
  GkLightRef      ref;
  struct GkLight *next;
  struct GkNode  *node;
  const char     *name;
  vec4           *ambient;
  vec3            direction;
  GkLightType     type;
  GkColor         color;
  int32_t         index;
  uint8_t         isvalid;
  uint8_t         enabled;
  uint8_t         readonly;
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
gkUniformLights(struct GkScene   * __restrict scene,
                struct GkProgram * __restrict prog);

void
gkUniformLight(struct GkScene   * __restrict scene,
               GkLight          * __restrict light,
               struct GkProgram * __restrict prog,
               mat4                          transView);

void
gkUniformSingleLight(struct GkScene   * __restrict scene,
                     GkLight          * __restrict light,
                     struct GkProgram * __restrict prog,
                     mat4                          transView);


struct GkTransform*
gkLightTransform(GkLight *light);

void
gkLightPos(GkLight *light, vec3 position);

void
gkLightDir(GkLight *light, vec3 dir);

#endif /* gk_light_h */
