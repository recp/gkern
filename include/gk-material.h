/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_material_h
#define gk_material_h

#include "gk-common.h"
#include "gk-color.h"
#include "gk-texture.h"

struct GkScene;
struct GkModelInst;
struct GkProgInfo;

typedef enum GkMaterialType {
  GK_MATERIAL_PHONG    = 1,
  GK_MATERIAL_BLINN    = 2,
  GK_MATERIAL_LAMBERT  = 3,
  GK_MATERIAL_CONSTANT = 4
} GkMaterialType;

typedef enum GkColorMethod {
  GK_DISCARD       = 0,
  GK_ONLY_TEX      = 1,
  GK_ONLY_COLOR    = 2,
  GK_MIX_COLOR_TEX = 3
} GkColorMethod;

typedef struct GkColorOrTex {
  GkColor       color;
  GkTexture    *tex;
  GkColorMethod method;
} GkColorOrTex;

typedef struct GkTechnique {
  GkMaterialType      type;
  uint32_t            index; /* subroutine index */
  char               *subroutine;
  struct GkTechnique *next;
} GkTechnique;

typedef struct GkPhong {
  GkTechnique   base;
  GkColorOrTex  emission;
  GkColorOrTex  ambient;
  GkColorOrTex  diffuse;
  GkColorOrTex  specular;
  GkColorOrTex  reflective;
  GkColorOrTex  transparent;
  float         shininess;
  float         reflectivity;
  float         transparency;
  float         indexOfRefraction;
} GkPhong;

typedef struct GkLambert {
  GkTechnique   base;
  GkColorOrTex  emission;
  GkColorOrTex  ambient;
  GkColorOrTex  diffuse;
  GkColorOrTex  reflective;
  GkColorOrTex  transparent;
  float         reflectivity;
  float         transparency;
  float         indexOfRefraction;
} GkLambert;

typedef struct GkConstant {
  GkTechnique   base;
  GkColorOrTex  emission;
  GkColorOrTex  reflective;
  GkColorOrTex  transparent;
  float         reflectivity;
  float         transparency;
  float         indexOfRefraction;
} GkConstant;

typedef GkPhong GkBlinn;

typedef struct GkMaterial {
  GkTechnique   *technique;
  uint8_t        isvalid;
  uint8_t        enabled;
} GkMaterial;

GkPhong*
gkMaterialNewPhong(void);

GkBlinn*
gkMaterialNewBlinn(void);

GkLambert*
gkMaterialNewLambert(void);

GkConstant*
gkMaterialNewConstant(void);

void
gkUniformMaterial(struct GkProgInfo * __restrict pinfo,
                  struct GkMaterial * __restrict material);

#endif /* gk_material_h */
