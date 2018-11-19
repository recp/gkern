/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/prims/cube.h"
#include "../default/def_prog.h"
#include <limits.h>

GLfloat gk__verts_cube[] = {
  -0.5f, -0.5f, -0.5f,
   0.5f, -0.5f, -0.5f,
   0.5f,  0.5f, -0.5f,
  -0.5f,  0.5f, -0.5f,
  -0.5f, -0.5f,  0.5f,
   0.5f, -0.5f,  0.5f,
   0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f,  0.5f,
};

GLushort gk__verts_cube_ind[] = {
   0, 1, 2, 3,
   4, 5, 6, 7,
   0, 4, 1, 5,
   2, 6, 3, 7
};

GLuint gk__cube_vao = UINT_MAX;
GLuint gk__cube_vbo[2];

void
gkInitCube() {
  uint32_t vPOSITION;

  vPOSITION = 0;

  glGenVertexArrays(1, &gk__cube_vao);
  glGenBuffers(2, gk__cube_vbo);

  glBindVertexArray(gk__cube_vao);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1, 0);

  glBindBuffer(GL_ARRAY_BUFFER, gk__cube_vbo[0]);
  glEnableVertexAttribArray(vPOSITION);
  glVertexAttribPointer(vPOSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(gk__verts_cube),
               gk__verts_cube,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gk__cube_vbo[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(gk__verts_cube_ind),
               gk__verts_cube_ind,
               GL_STATIC_DRAW);
}

void
gkDrawBBox(GkScene * __restrict scene,
           GkBBox               bbox,
           mat4                 world) {
  GkPipeline *prog;
  vec3       size, center;
  mat4       tran = GLM_MAT4_IDENTITY_INIT;
  GLuint     currentProg;

  prog        = gk_prog_cube();
  currentProg = gkCurrentProgram();

  glUseProgram(prog->progId);

  if (gk__cube_vao == UINT_MAX)
    gkInitCube();
  else
    glBindVertexArray(gk__cube_vao);

  glm_vec_sub(bbox[1], bbox[0], size);
  glm_vec_center(bbox[1], bbox[0], center);

  glm_translate(tran, center);
  glm_scale(tran, size);

  /* glm_mat4_mul(world, tran, tran); */
  glm_mat4_mul(scene->camera->viewProj, tran, tran);

  gkUniformMat4(prog->mvpi, tran);

  glDrawElements(GL_LINE_LOOP,
                 4,
                 GL_UNSIGNED_SHORT,
                 NULL);
  glDrawElements(GL_LINE_LOOP,
                 4,
                 GL_UNSIGNED_SHORT,
                 (GLvoid *)(4 * sizeof(GLushort)));
  glDrawElements(GL_LINES,
                 8,
                 GL_UNSIGNED_SHORT,
                 (GLvoid *)(8 * sizeof(GLushort)));

  glUseProgram(currentProg);
}

void
gkReleaseCube() {
  if (gk__cube_vao == UINT_MAX)
    return;

  glDeleteBuffers(2, gk__cube_vbo);
  glDeleteVertexArrays(1, &gk__cube_vao);

  gk__cube_vao = UINT_MAX;
}
