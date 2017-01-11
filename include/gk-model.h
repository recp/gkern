/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_model_h
#define gk_model_h

struct GkModelBase;
struct GkModel;
struct GkModelInst;

typedef void (*gkOnDraw)(struct GkModelBase * model,
                         struct GkModelInst * instance,
                         bool finished);

typedef void (*gkOnClick)(struct GkModelInst * instance,
                          float x,
                          float y);

typedef struct GkFinalMatrix {
  uint32_t refc;
  mat4     cmvp;  /* model view projection matrix */
  mat4     cnmat; /* cached normal matrix   */
} GkFinalMatrix;

/* some geometries or nodes may not have matrix, 
   so they will use parent's one. */
typedef struct GkMatrix {
  uint32_t       refc;
  uint32_t       cmatIsValid;
  /* 
   cached MVP and normal matrix: proj * view * model (GkMatrix->cmat)
   because multiple models may share same mvp matrix, to save space we use
   pointer space in every matrix, GkMatrix is allocating by malloc so to meet
   aligment requirements this space will be wasted anyway.
   
   dont change order because mat4 requires 16 byte aligment!
   */
  GkFinalMatrix *fmat;
  mat4           matrix;
  mat4           cmat;  /* cached matrix multiplied with parent */
} GkMatrix;

/* cache program infos, some nodes may use different program and shaders */
typedef struct GkProgInfo {
  uint32_t refc;  /* reference count        */
  GLint    prog;  /* program                */
  GLint    mvpi;  /* matrix loc             */
  GLint    nmi;   /* normal matrix loc      */
} GkProgInfo;

typedef struct GkGLEvents {
  gkOnDraw  onDraw;
  gkOnClick onClick;
} GkGLEvents;

typedef struct GkModelInstList {
  struct GkModelInst *instance;
  uint64_t            instanceCount;
} GkModelInstList;

typedef struct GkModelBase {
  uint64_t         flags;
  GkProgInfo      *pinfo;
  GkMatrix        *matrix;
  GkGLEvents      *events;
  GkModelInstList *instances;
} GkModelBase;

typedef struct GkModel {
  GkModelBase base;
  GLuint     *vbo;
  GLuint      vao;
  GLsizei     vboCount;
  GLsizei     count;
  GLenum      mode;
} GkModel;

typedef struct GkComplexModel {
  GkModelBase base;
  GLuint     *vao;
  GLuint     *vbo;
  GLsizei    *count;
  GLenum     *modes;
  GLsizei     vaoCount;
  GLsizei     vboCount;
} GkComplexModel;

typedef struct GkModelInst {
  GkModelBase        *model;
  GkMatrix           *matrix;
  GkGLEvents         *events;
  struct GkModelInst *next;
  uint64_t            flags;
  char                data[]; /* private field */
} GkModelInst;

#endif /* gk_model_h */
