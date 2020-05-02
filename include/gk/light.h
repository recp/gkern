/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef gk_light_h
#define gk_light_h

#include "common.h"
#include "color.h"
#include <cglm/cglm.h>

struct GkScene;
struct GkNode;
struct GkPipeline;
struct GkColorOrTex;
struct GkCamera;
struct GkTransform;

typedef enum GkLightType {
  GK_LIGHT_TYPE_AMBIENT     = 1,
  GK_LIGHT_TYPE_DIRECTIONAL = 2,
  GK_LIGHT_TYPE_POINT       = 3,
  GK_LIGHT_TYPE_SPOT        = 4,
  GK_LIGHT_TYPE_CUSTOM      = 5
} GkLightType;

typedef enum GkLightFlags {
  GK_LIGHTF_NONE          = 0,
  GK_LIGHTF_TRANSFORMED   = 1 << 1,
  GK_LIGHTF_DISABLED      = 1 << 2
} GkLightFlags;

/* for scene, because node can have multiple light instancs */
typedef struct GkLightRef {
  struct GkLightRef *prev;
  struct GkLightRef *next;
} GkLightRef;

typedef struct GkLight {
  GkLightRef       ref;
  struct GkLight  *next;
  struct GkNode   *node;
  struct GkCamera *camera;
  const char      *name;
  vec4            *ambient;
  vec3             dir;
  vec3             defdir;
  GkLightType      type;
  GkColor          color;
  int32_t          index;
  uint8_t          isvalid;
  GkLightFlags     flags;
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
  float   falloffAngle;
  float   cutoffCosine;
  float   cutoffExp;
} GkSpotLight;

void
gkUniformLights(struct GkScene   * __restrict scene,
                struct GkPipeline * __restrict prog);

void
gkUniformLight(struct GkScene   * __restrict scene,
               GkLight          * __restrict light,
               struct GkPipeline * __restrict prog,
               mat4                          transView);

void
gkUniformSingleLight(struct GkScene   * __restrict scene,
                     GkLight          * __restrict light,
                     struct GkPipeline * __restrict prog);

void
gkApplyTransformToLight(struct GkScene   * __restrict scene,
                        GkLight          * __restrict light,
                        struct GkPipeline * __restrict prog);

void
gkShadowMatrix(struct GkScene *scene,
               GkLight        *light,
               mat4            viewProj);

struct GkTransform*
gkLightTransform(GkLight *light);

void
gkLightPos(struct GkScene *scene, GkLight *light, vec3 position);

void
gkLightDir(struct GkScene *scene, GkLight *light, vec3 dir);

void
gkLightDirWorld(struct GkScene *scene, GkLight *light, vec3 dir);

void
gkLightRotation(struct GkScene *scene,
                GkLight        *light,
                vec3            right,
                vec3            up,
                vec3            fwd);

#endif /* gk_light_h */
