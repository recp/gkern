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

struct GkScene;
struct GkModelInst;

typedef enum GkMaterialType {
  GK_MATERIAL_PHONG    = 1,
  GK_MATERIAL_BLINN    = 2,
  GK_MATERIAL_LAMBERT  = 3,
  GK_MATERIAL_CONSTANT = 4
} GkMaterialType;

typedef struct GkColorOrTex {
  GkColor color;
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
gkMaterialNewPhong();

GkBlinn*
gkMaterialNewBlinn();

GkLambert*
gkMaterialNewLambert();

GkConstant*
gkMaterialNewConstant();

void
gkUniformMaterial(struct GkScene     * __restrict scene,
                  struct GkModelInst * __restrict modelInst);

#endif /* gk_material_h */
