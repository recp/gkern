/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk/gk.h"
#include "../include/gk/image.h"
#include "../include/gk/texture.h"
#include <OpenGL/gl3ext.h>

void
gkTexLoad(GkTexture * __restrict tex,
          bool                   ismutable) {
  GkImageMip *mip;
  GkImage    *image;
  GLuint      maxLevel;

  if (!(image = tex->image)) {
#ifdef DEBUG
    printf("invalid param: set and attach an image to texture!");
#endif
    return;
  }

  if (!tex->index)
    glGenTextures(1, &tex->index);

  glBindTexture(tex->target, tex->index);

  maxLevel = 0;
  mip      = &image->mips;
  switch (tex->target) {
    case GL_TEXTURE_1D: {
      glTexStorage1D(tex->target,
                     image->mipLevels,
                     tex->internalFormat,
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
        maxLevel++;
      } while (mip);
      break;
    }
    case GL_TEXTURE_1D_ARRAY: {
      glTexStorage2D(tex->target,
                     image->mipLevels,
                     tex->internalFormat,
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
        maxLevel++;
      } while (mip);
      break;
    }
    case GL_TEXTURE_2D: {
      glTexStorage2D(GL_TEXTURE_2D,
                     image->mipLevels,
                     tex->internalFormat,
                     image->mips.width,
                     image->mips.height);

      do {
        glTexSubImage2D(GL_TEXTURE_2D,
                        mip->level,
                        0,
                        0,
                        mip->width,
                        mip->height,
                        image->format,
                        image->type,
                        mip->data);
        mip = mip->next;
        maxLevel++;
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
                       tex->internalFormat,
                       mip->width,
                       mip->height,
                       0,
                       image->format,
                       image->type,
                       ptr + image->sliceStride * face);
        }
        mip = mip->next;
        maxLevel++;
      } while (mip);
      break;
    }
    case GL_TEXTURE_2D_ARRAY: {
      glTexStorage3D(tex->target,
                     image->mipLevels,
                     tex->internalFormat,
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
        maxLevel++;
      } while (mip);
      break;
    }
    case GL_TEXTURE_CUBE_MAP_ARRAY: {
      glTexStorage3D(tex->target,
                     image->mipLevels,
                     tex->internalFormat,
                     image->mips.width,
                     image->mips.height,
                     image->slices);
      break;
    }
    case GL_TEXTURE_3D: {
      glTexStorage3D(tex->target,
                     image->mipLevels,
                     tex->internalFormat,
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
        maxLevel++;
      } while (mip);
      break;
    }
    default:
    assert("unsupported image!");
  }

  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAX_LEVEL,
                  maxLevel);
}
