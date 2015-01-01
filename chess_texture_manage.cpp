/*
 * @file:        chess_texture_manage.cpp
 * @author:      matthiascy(matthias_cy@outlook.com)
 * @date:        2014-12-20
 * @description: 
 */

#ifdef _WINDOWS
#include <Windows.h>
#endif

#include <iostream>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"

#include "chess_tga_image.h"
#include "chess_texture_manage.h"

TextureMgr::TextureMgr()
{

}

TextureMgr::~TextureMgr()
{

}
int TextureMgr::convertStrToFilter(char *str)
{
  if (!strcmp(strupr(str), "LOW"))
    return FILTER_LOW;
  else if (!strcmp(strupr(str), "MED"))
    return FILTER_MED;
  else if (!strcmp(strupr(str), "HI"))
    return FILTER_HI; 
  else if (!strcmp(strupr(str), "BEST"))
    return FILTER_BEST;
  else
    return FILTER_LOW;
}

int TextureMgr::convertStrToWrap(char *str)
{
  if (!strcmp(strupr(str), "REPEAT"))
    return GL_REPEAT;
  else if (!strcmp(strupr(str), "CLAMP"))
    return GL_CLAMP;
  else if (!strcmp(strupr(str), "CLAMPEDGE"))
    return GL_CLAMP_TO_EDGE;
  else if (!strcmp(strupr(str), "CLAMPBORDER"))
    return GL_CLAMP_TO_BORDER;
  else if (!strcmp(strupr(str), "MIRROR"))
    return GL_MIRRORED_REPEAT;
  else
    return GL_REPEAT;
}

int TextureMgr::convertStrToTexEnv(char *str)
{
  if (!strcmp(strupr(str), "REPLACE"))
    return GL_REPLACE;
  else if (!strcmp(strupr(str), "MODULATE"))
    return GL_MODULATE;
  else if (!strcmp(strupr(str), "DECAL"))
    return GL_DECAL;
  else if (!strcmp(strupr(str), "BLEND"))
    return GL_BLEND;
  else if (!strcmp(strupr(str), "ADD"))
    return GL_ADD;
  else
    return GL_REPLACE;
}

bool TextureMgr::loadTextures(char *textureDataFile)
{
  // Texture data file format:
  // C++ style '//' at start of line is a comment
  // texturefilename   low/med/high/bestfiltermode

  FILE *fptr;
  fptr = fopen(textureDataFile, "r");

  if (!fptr)
    return false;

  numberTextures = 0;

  // read to EOF
  while ((!feof(fptr)) && (numberTextures < MAX_TEXTURES)) {
    char line[255];
    fgets(line, 255, fptr);

    // only read if this line is not a comment
    if ((line[0] != '/') && (line[1] != '/')) {
      char texName[30] = { '\0' };
      char filename[30] = { '\0' };
      char filterDetail[5] = { '\0' };
      char wrapMode[10] = { '\0' };
      char env[10] = { '\0' };

      sscanf(line, "%s %s %s %s %s", texName, filename, filterDetail, wrapMode, env);

      textureObjects[numberTextures].inUse = true;
      strcpy(textureObjects[numberTextures].textureName, texName);

      // load texture image data
      TargaImage imgData;

      imgData.load(filename);
      imgData.convertRGBToRGBA(255);

      // generate texture object
      glGenTextures(1, &(textureObjects[numberTextures].textureObjectID));

      // Make the texture the current one
      glBindTexture(GL_TEXTURE_2D, textureObjects[numberTextures].textureObjectID);

      int wrap = convertStrToWrap(wrapMode);
      int filter = convertStrToFilter(filterDetail);
      int texenv = convertStrToTexEnv(env);

      // Texture parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

      switch (filter) {
        case FILTER_LOW: {
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, texenv);

          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgData.getWidth(),
            imgData.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData.getImage());
        } break;

        case FILTER_MED: {
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, texenv);

          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgData.getWidth(),
            imgData.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData.getImage());
        } break;

        case FILTER_HI: {
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
          glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, texenv);

          gluBuild2DMipmaps(GL_TEXTURE_2D, 4, imgData.getWidth(),
            imgData.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, imgData.getImage());
        } break;

        case FILTER_BEST: {
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, texenv);

          gluBuild2DMipmaps(GL_TEXTURE_2D, 4, imgData.getWidth(),
            imgData.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, imgData.getImage());
        } break;
      }

      imgData.release();

      numberTextures++;
    }
  }

  return true;
}

bool TextureMgr::releaseTextures()
{
  for (int idx = 0; idx < numberTextures; idx++) {
    if (textureObjects[idx].inUse) {
      glDeleteTextures(1, &(textureObjects[idx].textureObjectID));
      textureObjects[idx].inUse = false;
    }
  }

  return true;
}

unsigned int TextureMgr::getTextureID(char *textureName)
{
  for (int idx = 0; idx < numberTextures; idx++) {
    if (!strcmp(textureName,textureObjects[idx].textureName))
      return idx;
  }

  return -1;
}

bool TextureMgr::bindTex(unsigned int textureID)
{
  if (textureObjects[textureID].inUse) {
    glBindTexture(GL_TEXTURE_2D, textureObjects[textureID].textureObjectID);
    return true;
  }

  return false;
}