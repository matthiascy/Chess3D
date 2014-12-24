/*
 * @file:        chess_tga_image.cpp
 * @author:      matthiascy(matthias_cy@outlook.com)
 * @date:        2014-12-20
 * @description: 
 */

#include <iostream>
#include "chess_tga_image.h"

TargaImage::TargaImage(): imageData(NULL)
{

}

TargaImage::~TargaImage()
{
  release();
}
void TargaImage::swapRedBlue()
{
  switch (colorDepth) {
    case 32: {
      unsigned char temp;
      RGBA_t* source = (RGBA_t*)imageData;

      for (int pixel = 0; pixel < (width * height); ++pixel) {
        temp = source[pixel].b;
        source[pixel].b = source[pixel].r;
        source[pixel].r = temp;
      }
             } break;

    case 24: {
      unsigned char temp;
      RGB_t* source = (RGB_t*)imageData;

      for (int pixel = 0; pixel < (width * height); ++pixel) {
        temp = source[pixel].b;
        source[pixel].b = source[pixel].r;
        source[pixel].r = temp;
      }
             } break;

    default:
      // ignore other color depths
      break;
  }
}

bool TargaImage::load(const char *filename)
{
  FILE *file = fopen(filename, "rb");

  if (!file)
    return false;

  TGAHeader tgaHeader;

  // read the TGA header
  fread(&tgaHeader, 1, sizeof(TGAHeader), file);

  // see if the image type is one that we support (RGB, RGB RLE, GRAYSCALE, GRAYSCALE RLE)
  if ( ((tgaHeader.imageTypeCode != TGA_RGB) && (tgaHeader.imageTypeCode != TGA_GRAYSCALE) && 
    (tgaHeader.imageTypeCode != TGA_RGB_RLE) && (tgaHeader.imageTypeCode != TGA_GRAYSCALE_RLE)) ||
    tgaHeader.colorMapType != 0) {
      fclose(file);
      return false;
  }

  // get image width and height
  width = tgaHeader.width;
  height = tgaHeader.height;

  // colormode -> 3 = BGR, 4 = BGRA
  int colorMode = tgaHeader.bpp / 8;

  // we don't handle less than 24 bit
  if (colorMode < 3) {
    fclose(file);
    return false;
  }

  imageSize = width * height * colorMode;

  // allocate memory for TGA image data
  imageData = new unsigned char[imageSize];

  // skip past the id if there is one
  if (tgaHeader.idLength > 0)
    fseek(file, SEEK_CUR, tgaHeader.idLength);

  // read image data
  if (tgaHeader.imageTypeCode == TGA_RGB ||
    tgaHeader.imageTypeCode == TGA_GRAYSCALE) {
      fread(imageData, 1, imageSize, file);
  } else {
    // this is an RLE compressed image
    unsigned char id;
    unsigned char length;
    RGBA_t color = { 0, 0, 0, 0 };
    unsigned int i = 0;

    while (i < imageSize) {
      id = fgetc(file);

      // see if this is run length data
      if (id >= 128) {
        // find the run length
        length = (unsigned char)(id - 127);

        // next 3 (or 4) bytes are the repeated values
        color.b = (unsigned char)fgetc(file);
        color.g = (unsigned char)fgetc(file);
        color.r = (unsigned char)fgetc(file);

        if (colorMode == 4)
          color.a = (unsigned char)fgetc(file);

        // save everything in this run
        while (length > 0) {
          imageData[i++] = color.b;
          imageData[i++] = color.g;
          imageData[i++] = color.r;

          if (colorMode == 4)
            imageData[i++] = color.a;

          --length;
        }
      } else {
        // the number of non RLE pixels
        length = unsigned char(id + 1);

        while (length > 0) {
          color.b = (unsigned char)fgetc(file);
          color.g = (unsigned char)fgetc(file);
          color.r = (unsigned char)fgetc(file);

          if (colorMode == 4)
            color.a = (unsigned char)fgetc(file);

          imageData[i++] = color.b;
          imageData[i++] = color.g;
          imageData[i++] = color.r;

          if (colorMode == 4)
            imageData[i++] = color.a;

          --length;
        }
      }
    }
  }

  fclose(file);

  switch(tgaHeader.imageTypeCode) {
    case TGA_RGB:
    case TGA_RGB_RLE: {
      if (3 == colorMode) {
        imageDataFormat = IMAGE_RGB;
        imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
        colorDepth = 24;
      } else {
        imageDataFormat = IMAGE_RGBA;
        imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
        colorDepth = 32;
      }
                      } break;

    case TGA_GRAYSCALE:
    case TGA_GRAYSCALE_RLE: {
      imageDataFormat = IMAGE_LUMINANCE;
      imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
      colorDepth = 8;
                            } break;
  }

  if ((tgaHeader.imageDesc & TOP_LEFT) == TOP_LEFT)
    flipVertical();

  // swap the red and blue components in the image data
  swapRedBlue();

  return (imageData != NULL);
}

bool TargaImage::flipVertical()
{
  if (!imageData)
    return false;

  if (colorDepth == 32) {
    RGBA_t* tmpBits = new RGBA_t[width];
    if (!tmpBits)
      return false;

    int lineWidth = width * 4;

    RGBA_t* top = (RGBA_t*)imageData;
    RGBA_t* bottom = (RGBA_t*)(imageData + lineWidth*(height-1));

    for (int i = 0; i < (height / 2); ++i) {
      memcpy(tmpBits, top, lineWidth); 
      memcpy(top, bottom, lineWidth);
      memcpy(bottom, tmpBits, lineWidth);

      top = (RGBA_t*)((unsigned char*)top + lineWidth);
      bottom = (RGBA_t* )((unsigned char*)bottom - lineWidth);
    }

    delete [] tmpBits;
    tmpBits = 0;
  } else if (colorDepth == 24) {
    RGB_t* tmpBits = new RGB_t[width];

    if (!tmpBits)
      return false;

    int lineWidth = width * 3;

    RGB_t* top = (RGB_t*)imageData;
    RGB_t* bottom = (RGB_t*)(imageData + lineWidth*(height-1));

    for (int i = 0; i < (height / 2); ++i) {
      memcpy(tmpBits, top, lineWidth); 
      memcpy(top, bottom, lineWidth);
      memcpy(bottom, tmpBits, lineWidth);

      top = (RGB_t*)((unsigned char*)top + lineWidth);
      bottom = (RGB_t*)((unsigned char*)bottom - lineWidth);
    }

    delete [] tmpBits;
    tmpBits = 0;
  }

  return true;
}

void TargaImage::release()
{
  delete [] imageData;
  imageData = NULL;
}

bool TargaImage::convertRGBToRGBA(unsigned char alphaValue)
{
  if ((colorDepth == 24) && (imageDataFormat == IMAGE_RGB)) {
    RGBA_t *newImage = new RGBA_t[width * height];

    if (!newImage)
      return false;

    RGBA_t *dest = newImage;
    RGB_t *src = (RGB_t*)imageData;

    for (int x = 0; x < height; x++) {
      for (int y = 0; y < width; y++) {
        dest->r = src->r;
        dest->g = src->g;
        dest->b = src->b;
        dest->a = alphaValue;

        ++src;
        ++dest;
      }
    }

    delete [] imageData;
    imageData = (unsigned char*)newImage;

    colorDepth = 32;
    imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
    imageDataFormat = IMAGE_RGBA;

    return true;
  }

  return false;
}

bool TargaImage::convertRGBAToRGB()
{
  if ((colorDepth == 32) && (imageDataFormat == IMAGE_RGBA)) {
    RGB_t *newImage = new RGB_t[width * height];

    if (!newImage)
      return false;

    RGB_t *dest = newImage;
    RGBA_t *src = (RGBA_t*)imageData;

    for (int x = 0; x < height; x++) {
      for (int y = 0; y < width; y++) {
        dest->r = src->r;
        dest->g = src->g;
        dest->b = src->b;

        ++src;
        ++dest;
      }
    }

    delete [] imageData;
    imageData = (unsigned char*)newImage;

    colorDepth = 24;
    imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
    imageDataFormat = IMAGE_RGB;

    return true;
  }

  return false;
}