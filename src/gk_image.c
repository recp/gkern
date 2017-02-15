/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"
#include "../include/gk-image.h"
#include <OpenGL/gl3ext.h>

void
gkImageLoad(GkImage * __restrict image,
            void    * __restrict data) {
  GkImageMip *mip;

  if (!image->index)
    glGenTextures(1, &image->index);

  mip = &image->mips;

  glBindTexture(image->target, image->index);
  switch (image->target) {
    case GL_TEXTURE_1D: {
      glTexStorage1D(image->target,
                     image->mipLevels,
                     image->iformat,
                     image->mips.width);
      do {
        glTexSubImage1D(GL_TEXTURE_1D,
                        mip->level,
                        0,
                        mip->width,
                        image->format,
                        image->type,
                        mip->data);
        mip = mip->next;
      } while (mip);
      break;
    }
    case GL_TEXTURE_1D_ARRAY: {
      glTexStorage2D(image->target,
                     image->mipLevels,
                     image->iformat,
                     image->mips.width,
                     image->slices);
      do {
        glTexSubImage2D(GL_TEXTURE_1D,
                        mip->level,
                        0,
                        0,
                        mip->width,
                        image->slices,
                        image->format,
                        image->type,
                        mip->data);
        mip = mip->next;
      } while (mip);
      break;
    }
    case GL_TEXTURE_2D: {
      glTexStorage2D(image->target,
                     image->mipLevels,
                     image->iformat,
                     image->mips.width,
                     image->mips.height);
      do {
        glTexSubImage2D(GL_TEXTURE_1D,
                        mip->level,
                        0,
                        0,
                        mip->width,
                        image->slices,
                        image->format,
                        image->type,
                        mip->data);
        mip = mip->next;
      } while (mip);
      break;
    }
    case GL_TEXTURE_CUBE_MAP: {
      do {
        GLubyte *ptr;
        int      face;

        ptr = mip->data;
        for (face = 0; face < 6; face++) {
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                       mip->level,
                       image->iformat,
                       mip->width,
                       mip->height,
                       0,
                       image->format,
                       image->type,
                       ptr + image->sliceStride * face);
        }
        mip = mip->next;
      } while (mip);
      break;
    }
    case GL_TEXTURE_2D_ARRAY: {
      glTexStorage3D(image->target,
                     image->mipLevels,
                     image->iformat,
                     image->mips.width,
                     image->mips.height,
                     image->slices);

      do {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                        mip->level,
                        0,
                        0,
                        0,
                        mip->width,
                        mip->height,
                        image->slices,
                        image->format,
                        image->type,
                        mip->data);
        mip = mip->next;
      } while (mip);
      break;
    }
    case GL_TEXTURE_CUBE_MAP_ARRAY: {
      glTexStorage3D(image->target,
                     image->mipLevels,
                     image->iformat,
                     image->mips.width,
                     image->mips.height,
                     image->slices);
      break;
    }
    case GL_TEXTURE_3D: {
      glTexStorage3D(image->target,
                     image->mipLevels,
                     image->iformat,
                     image->mips.width,
                     image->mips.height,
                     image->mips.depth);
      do {
        glTexSubImage3D(GL_TEXTURE_3D,
                        mip->level,
                        0,
                        0,
                        0,
                        mip->width,
                        mip->height,
                        mip->depth,
                        image->format,
                        image->type,
                        mip->data);
        mip = mip->next;
      } while (mip);
      break;
    }
    default:
      assert("unsupported image!");
  }

  glTexParameteriv(image->target,
                   GL_TEXTURE_SWIZZLE_RGBA,
                   (const GLint *)image->swizzle);
}
