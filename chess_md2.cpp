/*
 * @file:        chess_md2.cpp
 * @author:      matthiascy(matthias_cy@outlook.com)
 * @date:        2014-12-20
 * @description: 
 */

#include "chess_md2.h"
#include <cstdio>
#include <cmath>
#include <cfloat>

const float SPHERE_PADDING = 10.0;


typedef struct {
  int numVerts;
  int numTris;
  int numFrames;
  GLfloat minY;
  Sphere boundingSphere;
} ModelInfo;


bool loadModel(char *filename, Vertex* &verts, Mesh* &tris, TexCoord* &texCoords,
               ModelInfo &info, GLfloat scale);
void CalculateBoundingSphere(Sphere &sphere, GLfloat miny, GLfloat maxy);
  
MD2Instance::MD2Instance()
{
  currentFrame = 0;   // current keyframe 
  nextFrame = 1;      // next keyframe
  interpolation = 0.0;     // interpolation percent
  currentVerts = NULL;
  aniState = IDLE;
  rotateAngle = 0.0;
  data = NULL;
  numWeaponVertices = 0;
  numVertices = 0;
}    

MD2Instance::~MD2Instance()
{
  unload();
}

void MD2Instance::setAnimation(int state, int nextState)
{
  switch (state) {
    case RUN: {
      startFrame = 40;
      endFrame = 45;
    } break;

    case ATTACK: {
      startFrame = 46;
      endFrame = 53;
    } break;

    case PAIN1: {
      startFrame = 54;
      endFrame = 57;
    } break;

    case PAIN2: {
      startFrame = 58;
      endFrame = 61;
    } break;

    case PAIN3: {
      startFrame = 62;
      endFrame = 65;
    } break;

    case JUMP: {
      startFrame = 66;
      endFrame = 71;
    } break;

    case FLIPOFF: {
      startFrame = 72;
      endFrame = 83;
    } break;

    case SAULTE: {
      startFrame = 84;
      endFrame = 94;
    } break;

    case TAUNT: {
      startFrame = 95;
      endFrame = 111;
    } break;

    case WAVE: {
      startFrame = 112;
      endFrame = 122;
    } break;

    case POINT: {
      startFrame = 123;
      endFrame = 134;
    } break;

    case CROUCH_IDLE: {
      startFrame = 135;
      endFrame = 153;
    } break;

    case CROUCH_WALK: {
      startFrame = 154;
      endFrame = 159;
     } break;

    case CROUCH_ATTACK: {
      startFrame = 160;
      endFrame = 168;
    } break;

    case CROUCH_PAIN: {
      startFrame = 169;
      endFrame = 172;
    } break;

    case CROUCH_DEATH: {
      startFrame = 173;
      endFrame = 177;
    } break;

    case DEATH1: {
      startFrame = 178;
      endFrame = 183;
    } break;

    case DEATH2: {
      startFrame = 184;
      endFrame = 189;
    } break;

    case DEATH3: {
      startFrame = 190;
      endFrame = 197;
    } break;

    case IDLE: {
      startFrame = 0;
      endFrame = 39;
    } break;

    default:
      return;
  }

  if (aniState != state) {
    currentFrame = startFrame;
    nextFrame = startFrame + 1;
  }

  aniState = state;
  nextAniState = nextState;
}

void MD2Instance::setAnimationCustom(int start, int end)
{
  if (start < 0)
    start = 0;
  if (start >= numFrames)
    start = numFrames - 1;

  if (end < 0)
    end = 0;
  if (end >= numFrames)
    end = numFrames - 1;

  startFrame = start;
  endFrame = end;

  aniState = _CUSTOM;
  nextAniState = _CUSTOM;
}

void MD2Instance::animate(float seconds)
{
  if (startFrame > currentFrame)
    currentFrame = startFrame;

  if (startFrame < 0 || endFrame < 0)
    return;

  if (startFrame >= numFrames || endFrame >= numFrames)
    return;

  // increase percentage of interpolation between frames
  interpolation += KEYFRAMES_PER_S * seconds;

  if (interpolation >= 1.0) {
    interpolation = 0.0f;
    currentFrame++;
    nextFrame = currentFrame + 1;

    if (currentFrame > endFrame) {
      if (nextAniState == _REPEAT || aniState == _CUSTOM) {
        currentFrame = startFrame;
        nextFrame = currentFrame + 1;
      } else if (nextAniState == _STATIC) {
        currentFrame = nextFrame = startFrame = endFrame;
        aniState = _STATIC;
      } else {
        setAnimation(nextAniState, _REPEAT);
      }
    }

    if (nextFrame > endFrame)
      nextFrame = startFrame;
  }

  if ((nextAniState == _STATIC) && (nextFrame == startFrame))
    nextFrame = endFrame;

  data->animate(currentVerts, weaponVerts, currentFrame, nextFrame, interpolation);
}

void MD2Instance::render()
{
  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(rotateAngle, 0, 1, 0);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, currentVerts);
  glTexCoordPointer(2, GL_FLOAT, 0, data->texCoords);

  glBindTexture(GL_TEXTURE_2D, data->texID);
  glDrawArrays(GL_TRIANGLES, 0, numVertices);

  if (weaponVerts) {
    glVertexPointer(3, GL_FLOAT, 0, weaponVerts);
    glTexCoordPointer(2, GL_FLOAT, 0, data->weaponTexCoords);

    glBindTexture(GL_TEXTURE_2D, data->weaponTexID);
    glDrawArrays(GL_TRIANGLES, 0, numWeaponVertices);
  }

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glPopMatrix();
}

// unload()
// desc: unloads model data from memory
void MD2Instance::unload()
{
  delete [] currentVerts;
  currentVerts = NULL;
  delete [] weaponVerts;
  weaponVerts = NULL;
}


void MD2Instance::move(GLfloat x, GLfloat y, GLfloat z)
{
  pos.x = x;
  pos.y = y - miny;
  pos.z = z;

  boundingSphere.center.x = x;
  boundingSphere.center.y = y + boundingSphere.radius - SPHERE_PADDING;
  boundingSphere.center.z = z;
}

// MD2Data constructor
MD2Data::MD2Data()
{
  tris = NULL;       // triangle indices
  texCoords = NULL;  // texture coordinate indices
  verts = NULL;      // vertices
  texID = 0;         // skin/texture
  numVerts = 0;
  numTris = 0;
  numFrames = 0;

  weaponTris = NULL;
  weaponTexCoords = NULL;
  weaponVerts = NULL;
  numWeaponVerts = 0;
  numWeaponTris = 0;
  weaponTexID = 0;
}    

// MD2Data destructor
MD2Data::~MD2Data()
{
  unload();
}

// MD2Data::load()
// access: public
// desc: loads model and skin
bool MD2Data::load(char *modelFile, char *skinFile, char *weaponFile,
                   char *weaponSkin, float scale)
{
  ModelInfo info;

  loadModel(modelFile, verts, tris, texCoords, info, scale);
  numVerts = info.numVerts;
  numTris = info.numTris;
  numFrames = info.numFrames;
  miny = info.minY;
  boundingSphere = info.boundingSphere;

  if (weaponFile) {
    loadModel(weaponFile, weaponVerts, weaponTris, weaponTexCoords, info, scale);
    numWeaponVerts = info.numVerts;
    numWeaponTris = info.numTris;
  }

  TargaImage image;

  image.load(skinFile);
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.getWidth(), image.getHeight(),
    GL_RGB, GL_UNSIGNED_BYTE, image.getImage());
  image.release();

  if (weaponSkin) {
    image.load(weaponSkin);
    glGenTextures(1, &weaponTexID);
    glBindTexture(GL_TEXTURE_2D, weaponTexID);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.getWidth(), image.getHeight(),
      GL_RGB, GL_UNSIGNED_BYTE, image.getImage());
    image.release();
  }
  return true;
}

MD2Instance* MD2Data::getInstance()
{
  MD2Instance *instance = new MD2Instance();

  instance->currentVerts = new Vertex[numTris * 3];
  instance->weaponVerts = new Vertex[numWeaponTris * 3];
  instance->currentFrame = 0;
  instance->nextFrame = 1;
  instance->interpolation = 0.0;
  instance->numFrames = numFrames;
  instance->numVertices = numTris * 3;
  instance->numWeaponVertices = numWeaponTris * 3;
  instance->data = this;
  instance->boundingSphere = boundingSphere;
  instance->miny = miny;

  // animate at least one frame to make sure that currentVerts gets initialized
  instance->setAnimation(MD2Instance::IDLE);
  instance->animate(0);

  return instance;
}


void MD2Data::animate(Vertex *pVerts, Vertex *pWeaponVerts, int curFrame,
                      int nextFrame, float interpol)
{
  Vertex *vList;              // current frame vertices
  Vertex *nextVList;          // next frame vertices
  int i;                                  // index counter
  float x1, y1, z1;                  // current frame point values
  float x2, y2, z2;                  // next frame point values

  vList = &verts[numVerts * curFrame];
  nextVList = &verts[numVerts * nextFrame];

  for (i = 0; i < numTris * 3; i++) {
    Vertex &vertex = pVerts[i];
    // get first points of each frame
    x1 = vList[tris[i/3].meshIndex[i%3]].x;
    y1 = vList[tris[i/3].meshIndex[i%3]].y;
    z1 = vList[tris[i/3].meshIndex[i%3]].z;
    x2 = nextVList[tris[i/3].meshIndex[i%3]].x;
    y2 = nextVList[tris[i/3].meshIndex[i%3]].y;
    z2 = nextVList[tris[i/3].meshIndex[i%3]].z;

    // store first interpolated vertex of triangle
    vertex.x = x1 + interpol * (x2 - x1);
    vertex.y = y1 + interpol * (y2 - y1);
    vertex.z = z1 + interpol * (z2 - z1);
  }

  if (weaponVerts) {
    vList = &weaponVerts[numWeaponVerts * curFrame];
    nextVList = &weaponVerts[numWeaponVerts * nextFrame];

    for (i = 0; i < numWeaponTris * 3; i++) {
      Vertex &vertex = pWeaponVerts[i];
      // get first points of each frame
      x1 = vList[weaponTris[i/3].meshIndex[i%3]].x;
      y1 = vList[weaponTris[i/3].meshIndex[i%3]].y;
      z1 = vList[weaponTris[i/3].meshIndex[i%3]].z;
      x2 = nextVList[weaponTris[i/3].meshIndex[i%3]].x;
      y2 = nextVList[weaponTris[i/3].meshIndex[i%3]].y;
      z2 = nextVList[weaponTris[i/3].meshIndex[i%3]].z;

      // store first interpolated vertex of triangle
      vertex.x = x1 + interpol * (x2 - x1);
      vertex.y = y1 + interpol * (y2 - y1);
      vertex.z = z1 + interpol * (z2 - z1);
    }
  }
}

// unload()
// desc: unloads model data from memory
void MD2Data::unload()
{
  delete [] tris;
  tris = NULL;
  delete [] verts;
  verts = NULL;
  delete [] texCoords;
  texCoords = NULL;

  delete [] weaponTris;
  weaponTris = NULL;
  delete [] weaponVerts;
  weaponVerts = NULL;
  delete [] weaponTexCoords;
  weaponTexCoords = NULL;

  glDeleteTextures(1, &texID);
}


bool loadModel(char *filename, Vertex* &verts, Mesh* &tris, TexCoord* &texCoords,
               ModelInfo &info, GLfloat scale)
{
  // open the model file
  FILE *file = fopen(filename, "rb");
  if (file == NULL)
    return false;

  ModelHeader header;

  fread(&header, sizeof(ModelHeader), 1, file);
  verts = new Vertex[header.numVerts * header.numFrames];

  info.numVerts = header.numVerts;
  info.numTris = header.numTris;
  info.numFrames = header.numFrames;

  char *buffer = new char[header.numFrames * header.framesize];
  fseek(file, header.offsetFrames, SEEK_SET);
  fread(buffer, header.numFrames, header.framesize, file);

  int i, j;
  Frame *frame;
  Vertex *pVerts;
  info.minY = FLT_MAX;
  GLfloat maxy = FLT_MIN;

  for (j = 0; j < header.numFrames; ++j) {
    frame = (Frame*)&buffer[header.framesize * j];
    pVerts = (Vertex*)&verts[header.numVerts * j];
    for (i = 0; i < header.numVerts; i++) {
      pVerts[i].x = scale * (frame->scale[0] * frame->fp[i].v[0] + frame->translate[0]); 
      pVerts[i].z = scale * (frame->scale[1] * frame->fp[i].v[1] + frame->translate[1]); 
      pVerts[i].y = scale * (frame->scale[2] * frame->fp[i].v[2] + frame->translate[2]); 

      if (j == 0) {
        if (pVerts[i].y < info.minY)
          info.minY = pVerts[i].y;
        if (pVerts[i].y > maxy)
          maxy = pVerts[i].y;
      }
    }
  }

  CalculateBoundingSphere(info.boundingSphere, info.minY, maxy);

  tris = new Mesh[header.numTris];
  fseek(file, header.offsetTris, SEEK_SET);
  fread(tris, header.numTris, sizeof(Mesh), file);

  TexCoordIdx *stTemp = new TexCoordIdx[header.numTex];
  texCoords = new TexCoord[header.numTris * 3];
  fseek(file, header.offsetTex, SEEK_SET);
  fread(stTemp, header.numTex, sizeof(TexCoordIdx), file);

  int index = 0;
  for (i = 0; i < header.numTris; i++) {
    texCoords[index].s = (float)stTemp[tris[i].texIndex[0]].s / header.skinwidth;
    texCoords[index++].t = 1.0f - (float)stTemp[tris[i].texIndex[0]].t / header.skinheight;
    texCoords[index].s = (float)stTemp[tris[i].texIndex[1]].s / header.skinwidth;
    texCoords[index++].t = 1.0f - (float)stTemp[tris[i].texIndex[1]].t / header.skinheight;
    texCoords[index].s = (float)stTemp[tris[i].texIndex[2]].s / header.skinwidth;
    texCoords[index++].t = 1.0f - (float)stTemp[tris[i].texIndex[2]].t / header.skinheight;
  }

  // close file and free memory
  fclose(file);
  delete[] buffer;

  return true;
}

// this is a bit of a hack, since the sphere isn't guaranteed to completely
// surround the model. However, the results for this demo are satisfactory
void CalculateBoundingSphere(Sphere &sphere, GLfloat miny, GLfloat maxy)
{
  sphere.center.x = 0;
  sphere.center.y = (maxy + miny) / 2.0f;
  sphere.center.z = 0;
  sphere.radius = maxy - sphere.center.y + SPHERE_PADDING;
}