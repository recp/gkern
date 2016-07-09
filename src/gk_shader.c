/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk-shader.h"

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>

#include "fs_utils.h"

typedef void (*gkGLUniformFnf)(GLint location,
GLsizei count,
GLboolean transpose,
const GLfloat *value);

typedef void (*gkGLUniformFnd)(GLint location,
GLsizei count,
GLboolean transpose,
const GLdouble *value);

static gkGLUniformFnf uniformMatrixFnf[][5] = {
  {0},
  {0},
  {
    NULL,
    NULL,
    glUniformMatrix2fv,
    glUniformMatrix2x3fv,
    glUniformMatrix2x4fv
  },
  {
    NULL,
    NULL,
    glUniformMatrix3x2fv,
    glUniformMatrix3fv,
    glUniformMatrix3x4fv
  },
  {
    NULL,
    NULL,
    glUniformMatrix4x2fv,
    glUniformMatrix4x3fv,
    glUniformMatrix4fv
  }
};

static gkGLUniformFnd uniformMatrixFnd[][5] = {
  {0},
  {0},
  {
    NULL,
    NULL,
    glUniformMatrix2dv,
    glUniformMatrix2x3dv,
    glUniformMatrix2x4dv
  },
  {
    NULL,
    NULL,
    glUniformMatrix3x2dv,
    glUniformMatrix3dv,
    glUniformMatrix3x4dv
  },
  {
    NULL,
    NULL,
    glUniformMatrix4x2dv,
    glUniformMatrix4x3dv,
    glUniformMatrix4dv
  }
};

void
gkShaderLogInfo(GLuint shaderId,
                FILE * __restrict file) {
  char * infoLog;
  GLint  infoLogLen;

  glGetShaderiv(shaderId,
                GL_INFO_LOG_LENGTH,
                &infoLogLen);

  infoLog = malloc(sizeof(*infoLog) * infoLogLen + 1);

  glGetShaderInfoLog(shaderId,
                     infoLogLen,
                     NULL,
                     infoLog);

  fprintf(file,
          " -- Info Log For Shader: %d -- \n",
          shaderId);

  fwrite(infoLog,
         1,
         infoLogLen,
         file);

  fprintf(file, " --------------------- \n");

  free(infoLog);
}

GLuint
gkShaderLoad(GLenum shaderType,
             const char * source) {
  FILE  *logFile;
  GLuint shaderId;
  GLint  status;

  logFile  = stderr;
  shaderId = glCreateShader(shaderType);

  if(shaderId == 0) {
    fprintf(logFile, "Couldn't create shader!");
    return shaderId;
  }

  glShaderSource(shaderId,
                 1,
                 (const GLchar **)&source,
                 NULL);

  glCompileShader(shaderId);

  glGetShaderiv(shaderId,
                GL_COMPILE_STATUS,
                &status);

  if (status == GL_FALSE) {
    gkShaderLogInfo(shaderId, logFile);

    glDeleteShader(shaderId);
    shaderId = 0;
  }

  return shaderId;
}

GLuint
gkShaderLoadFromFile(GLenum shaderType,
                     const char * path) {
  char * source;
  GLuint shaderId;
  int    ret;

  shaderId = 0;
  ret      = fsReadfile(path, "rb", &source);
  
  if (ret == 0)
    shaderId = gkShaderLoad(shaderType, source);

  free(source);

  return shaderId;
}

int
gkShaderLoadFromFolder(const char * __restrict path,
                       GkShader ** __restrict dest) {
  GkShader      *shader;
  GkShader      *lastShader;
  GkShader      *currShader;
  DIR           *dir;
  struct dirent *ent;
  char          *cwdir;
  int            count;

  count      = 0;
  shader     = NULL;
  currShader = NULL;
  lastShader = NULL;
  cwdir      = getcwd(NULL, 0);

  if (chdir(path) < 0) {
    fprintf(stderr, "Unable change current work dir to specified path!");
    return 0;
  }

  if ((dir = opendir(path))) {

    while ((ent = readdir(dir))) {

      const char *fname;
      const char *ext;
      const char *dot;
      GLuint      shaderType;

      fname = ent->d_name;

      dot = strrchr(fname, '.');
      if (!dot || dot == fname || strcmp(fname, "..") == 0)
        continue;

      ext = dot + 1;

      if (strcmp(ext, "vert") == 0) {
        shaderType = GL_VERTEX_SHADER;
      } else if (strcmp(ext, "frag") == 0) {
        shaderType = GL_FRAGMENT_SHADER;
      } else if (strcmp(ext, "geom") == 0) {
        shaderType = GL_GEOMETRY_SHADER;
      } else if (strcmp(ext, "tesc") == 0) {
        shaderType = GL_TESS_CONTROL_SHADER;
      } else if (strcmp(ext, "tese") == 0) {
        shaderType = GL_TESS_EVALUATION_SHADER;
      } else {
        continue;
      }

      currShader = calloc(sizeof(*shader), 1);
      currShader->shaderType = shaderType;
      currShader->shaderId   = gkShaderLoadFromFile(shaderType, fname);
      currShader->isValid    = currShader->shaderId > 0;

      if (lastShader)
        lastShader->next = currShader;

      if (!shader)
        shader = currShader;

      lastShader = currShader;
    }
  } else {
    shader = NULL;
    count  = -1;
  }

  /* restore cwdir */
  chdir(cwdir);

  *dest = shader;
  
  return count;
}

void
gkAttachShaders(GLuint program,
                GkShader * __restrict shaders) {
  GkShader *shaderIter;

  for (shaderIter = shaders;
       shaderIter;
       shaderIter = shaderIter->next) {

    if (shaderIter->isValid)
      glAttachShader(program, shaderIter->shaderId);
  }
}

void
gkUniformMatrix4f(GLint location, MkMatrix *matrix) {
  glUniformMatrix4fv(location,
                     1,
                     GL_FALSE,
                     MkMatrixVal(matrix));
}

void
gkUniformMatrix(GLint location, MkMatrix *matrix) {

  assert(matrix->rows < 5
         && matrix->columns < 5);

  switch (matrix->base.itemSize) {
    case sizeof(float):
      uniformMatrixFnf[matrix->rows][matrix->columns](location,
                                                      1,
                                                      GL_FALSE,
                                                      MkMatrixVal(matrix));
      break;

    case sizeof(double):
      uniformMatrixFnd[matrix->rows][matrix->columns](location,
                                                      1,
                                                      GL_FALSE,
                                                      MkMatrixVal(matrix));
      break;
    default:
      break;
  }
}
