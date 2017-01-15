/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_light_h
#define gk_light_h

#include <cglm.h>

typedef enum GkLightType {
  GK_LIGHT_TYPE_AMBIENT     = 1,
  GK_LIGHT_TYPE_DIRECTIONAL = 2,
  GK_LIGHT_TYPE_POINT       = 3,
  GK_LIGHT_TYPE_SPOT        = 4,
  GK_LIGHT_TYPE_CUSTOM      = 5
} GkLightType;

typedef struct GkLight {
  struct GkLight *next;
  GkLightType     type;
  float           intensity;
  GkColor         color;
} GkLight;

typedef GkLight GkAmbientLight;
typedef GkLight GkDirectionalLight;

typedef struct GkPointLight {
  GkLight base;
  float   constantAttenuation;
  float   linearAttenuation;
  float   quadraticAttenuation;
} GkPointLight;

typedef struct GkSpotLight {
  GkLight base;
  float   constantAttenuation;
  float   linearAttenuation;
  float   quadraticAttenuation;
  float   falloffAngle;
  float   falloffExponent;
} GkSpotLight;

#endif /* gk_light_h */
