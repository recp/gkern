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

#ifndef gk_geometry_h
#define gk_geometry_h

#include "common.h"
#include "program.h"
#include "material.h"
#include "bbox.h"
#include "transform.h"
#include "buffer.h"

#include <stdint.h>

struct GkGeometry;
struct GkGeometryInst;
struct GkContext;
struct GkSkin;
struct GkMorph;
struct GkInstanceMorph;
struct FListItem;

typedef void (*gkOnDraw)(struct GkGeometry     * geom,
                         struct GkGeometryInst * instance,
                         bool finished);

typedef void (*gkOnClick)(struct GkGeometryInst * instance,
                          float x,
                          float y);

typedef struct GkGLEvents {
  gkOnDraw  onDraw;
  gkOnClick onClick;
} GkGLEvents;

typedef struct GkGeometryInstList {
  struct GkGeometryInst *instance;
  uint64_t            instanceCount;
} GkGeometryInstList;

typedef struct GkVertexInputBind {
  struct GkVertexInputBind *next;
  struct GkVertexInput     *input;
  int32_t                   attribLocation;
  bool                      enabled;
} GkVertexInputBind;

typedef enum GkVertexAttachmentSemantic {
  GK_VERT_ATTACH_UNKNOWN = 0,
  GK_VERT_ATTACH_SKIN    = 1,
  GK_VERT_ATTACH_MORPH   = 2
} GkVertexAttachmentSemantic;

typedef struct GkVertexAttachment {
  struct GkVertexAttachment *next;
  GkVertexInputBind         *firstInput;
  GkVertexInputBind         *lastInput;
  uint32_t                   nInputs;
  GkVertexAttachmentSemantic semantic;
} GkVertexAttachment;

typedef struct GkPrimitive {
  struct GkPrimitive  *prev;
  struct GkPrimitive  *next;
  GkMaterial          *material;
  GkBindTexture       *bindTexture;
  GkMaterial          *activeMaterial;
  GkVertexAttachment   vertex;
  GkBBox               bbox; /* local */
  GLuint               flags;
  GLuint               vao;
  GLsizei              count;
  GLenum               mode;
  bool                 invalidateVertex:1;
  
  /* DEPRECATED: */
  GkGpuBuffer         *bufs;
  GLsizei              bufc;
} GkPrimitive;

typedef struct GkPrimInst {
  GkPrimitive            *prim;
  GkMaterial             *material;
  GkBindTexture          *bindTexture;
  GkMaterial             *activeMaterial;
  GkTransform            *trans;
  struct GkGeometryInst  *geomInst;
  GkVertexAttachment     *vertexAttachments;
  GkBBox                  bbox;
  uint32_t                maxJoint;
  bool                    hasMorph:1;
  bool                    hasSkin:1;
  bool                    invalidateVertex:1;
} GkPrimInst;

typedef enum GkGeometryFlags {
  GK_GEOM_FLAGS_NONE      = 0,
  GK_GEOM_FLAGS_DRAW_BBOX = 1 << 0,
} GkGeometryFlags;

typedef struct GkGeometry {
  GkPipeline         *prog;
  GkMaterial         *material;
  GkBindTexture      *bindTexture;
  GkGLEvents         *events;
  GkGeometryInstList *instances; /* TODO: */
  GkBBox              bbox;   /* local */
  vec3                center;
  uint32_t            flags;
  uint32_t            primc;
  GkPrimitive         prims[];
} GkGeometry;

typedef struct GkGeometryInst {
  struct GkGeometryInst  *next;
  GkGeometry             *geom;
  GkTransform            *trans;    /* readonly: don't set this manually */
  GkMaterial             *material; /* instances may use different materials */
  GkBindTexture          *bindTexture;
  GkMaterial             *activeMaterial;
  mat4                   *joints;
  mat4                   *jointsToDraw;
  GkGpuBuffer            *uboJoints;
  GkGpuBuffer            *uboTargetWeights;
  struct GkSkin          *skin;
  struct GkInstanceMorph *morpher;
  GkBBox                  bbox;
  vec3                    center;
  bool                    addedToScene;
  uint64_t                flags;
  int32_t                 primc;
  GkPrimInst              prims[];
} GkGeometryInst;

void
gk_geom_add(struct GkContext * __restrict ctx,
            GkGeometry       * __restrict geom,
            void             * __restrict source);

GkGeometry*
gk_geom_find(struct GkContext * __restrict ctx,
             void             * __restrict source);

GkGeometryInst*
gkMakeInstance(GkGeometry *geom);

GK_EXPORT
void
gkPrimAddBuffer(GkPrimitive * __restrict prim,
                GkGpuBuffer * __restrict buff);


GK_EXPORT
void
gkVertexAttrib(struct GkContext * __restrict ctx,
               GkPrimitive      * __restrict prim,
               GkGpuBuffer      * __restrict buff,
               const char       * __restrict attribName,
               GLenum                        type,
               uint32_t                      bound,
               size_t                        byteStride,
               size_t                        byteOffset);
GK_EXPORT
void
gkVertexAttribi(GkPrimitive * __restrict prim,
                GkGpuBuffer * __restrict buff,
                uint32_t                 attribIndex,
                uint32_t                 bound,
                size_t                   byteStride,
                size_t                   byteOffset);

#endif /* gk_geometry_h */
