/*
 * @file:        chess_md2.h
 * @author:      matthiascy(matthias_cy@outlook.com)
 * @date:        2014-12-20
 * @description: 
 */

#ifndef __CHESS_MD2_H__
#define __CHESS_MD2_H__

#include <windows.h>			// standard Windows app include
#include <gl/gl.h>				// standard OpenGL include
#include <gl/glu.h>				// OpenGL utilities

#include "chess_tga_image.h"


const float KEYFRAMES_PER_S = 8;

// a single vertex
typedef struct
{
  union {
    struct {
      GLfloat x, y, z;
    };
    GLfloat v[3];
  };
} Vertex;

typedef struct {
  Vertex center;
  GLfloat radius;
} Sphere;

// texture coordinate
typedef struct {
  float s;
  float t;
} TexCoord;

// texture coordinate index
typedef struct {
  short s;
  short t;
} TexCoordIdx;

// info for a single frame point
typedef struct {
  unsigned char v[3];
  unsigned char normalIndex;	// not used
} FramePoint;

// information for a single frame
typedef struct {
  float scale[3];
  float translate[3];
  char name[16];
  FramePoint fp[1];
} Frame;

// data for a single triangle
typedef struct {
  unsigned short meshIndex[3];		// vertex indices
  unsigned short texIndex[3];		// texture coordinate indices
} Mesh;

typedef struct {
  int ident;		 // identifies as MD2 file "IDP2"
  int version;	 // mine is 8
  int skinwidth;    // width of texture
  int skinheight;   // height of texture
  int framesize;    // number of bytes per frame
  int numSkins;     // number of textures
  int numVerts;       // number of points
  int numTex;        // number of texture
  int numTris;      // number of triangles
  int numGLcmds;
  int numFrames;    // total number of frames
  int offsetSkins;  // offset to skin names (64 bytes each)
  int offsetTex;     // offset of texture s-t values
  int offsetTris;   // offset of triangle mesh
  int offsetFrames; // offset of frame data (points)
  int offsetGLcmds; // type of OpenGL commands to use
  int offsetEnd;    // end of file
} ModelHeader;

class MD2Data;

class MD2Instance {
  friend MD2Data;

public:
  ~MD2Instance();       // destructor

  // render model with interpolation to get animation
  void setAnimationCustom(int startFrame, int endFrame);
  void setAnimation(int state, int nextState = _REPEAT);
  void animate(float seconds);

  void render();

  // free memory of model
  void unload();

  // retrieve animation state of model
  int getAnimation() { return aniState; }

  const Sphere &getBoundingSphere() { return boundingSphere; }

  void move(GLfloat x, GLfloat y, GLfloat z);
  void rotate(GLfloat angle) { rotateAngle = angle; }

  // model animation states
  enum {
    IDLE, RUN, ATTACK,
    PAIN1, PAIN2, PAIN3,
    JUMP, FLIPOFF, SAULTE, TAUNT, WAVE, POINT,
    CROUCH_IDLE, CROUCH_WALK, CROUCH_ATTACK, CROUCH_PAIN, CROUCH_DEATH,
    DEATH1, DEATH2, DEATH3, _REPEAT, _CUSTOM, _STATIC
  };

protected:
  MD2Instance();        // constructor

private:
  int aniState;        // current model animation state
  int nextAniState;    // state to automatically transition to after the current state is finished
  int currentFrame; // current frame # in animation
  int nextFrame;    // next frame # in animation
  int startFrame;   // first frame in current animation
  int endFrame;     // last frame in current animation

  Sphere boundingSphere;
  float interpolation;          // percent through current frame

  int numVertices;
  int numWeaponVertices;
  int numFrames;

  Vertex *currentVerts;    // working vertex list
  Vertex *weaponVerts;
  MD2Data *data;

  GLfloat miny;
  Vertex pos;
  GLfloat rotateAngle;
};

class MD2Data {
  friend MD2Instance;

public:
  MD2Data();        // constructor
  ~MD2Data();       // destructor

  // load model and skin/texture at the same time
  bool load(char *modelFile, char *skinFile, char *weaponFile = NULL,
    char *weaponSkin = NULL, float scale = 1.0f);

  // free memory of model
  void unload();

  MD2Instance *getInstance();

protected:
  void animate(Vertex *pVerts, Vertex *pWeaponVerts, int curFrame,
    int nextFrame, float interpol);
  const Sphere &getBoundingSphere() { return boundingSphere; }

private:
  Sphere boundingSphere;
  GLfloat miny;

  // main mesh data
  Mesh *tris;           // triangle list
  TexCoord *texCoords;  // texture coordinate list
  Vertex *verts;           // vertex list
  int numVerts;
  int numTris;
  GLuint texID;  // texture ID
  int numFrames;

  // weapon mesh data
  Mesh *weaponTris;           // triangle list
  TexCoord *weaponTexCoords;  // texture coordinate list
  Vertex *weaponVerts;           // vertex list
  int numWeaponVerts;
  int numWeaponTris;
  GLuint weaponTexID;  // texture ID
};

#endif // __CHESS_MD2_H__