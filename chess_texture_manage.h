#ifndef __CHESS_TEXTURE_MANAGE_H__
#define __CHESS_TEXTURE_MANAGE_H__

// this class provides a central location for loading and binding 2D textures

class TargaImage;

#define MAX_TEXTURES 64

// a texture object used in the texture manager
struct TextureObj {
  unsigned int textureObjectID;   // texture object
  bool inUse; // is the object loaded?
  char textureName[28]; // name of texture
};

class TextureMgr {
private:
  TextureObj textureObjects[MAX_TEXTURES];    // array of textures
  int numberTextures; // number of textures in array

  enum FilterModes {
    FILTER_NONE,
    FILTER_LOW,
    FILTER_MED,
    FILTER_HI,
    FILTER_BEST
  };

  enum WrapModes {
    WRAP_NONE,
    WRAP_REPEAT,
    WRAP_CLAMP,
    WRAP_CLAMPEDGE,
    WRAP_CLAMPBORDER,
    WRAP_CLAMPMIRROR
  };

  enum TexEnvs {
    ENV_NONE,
    ENV_REPLACE,
    ENV_MODELATE,
    ENV_DECAL,
    ENV_BLEND,
    ENV_ADD
  };

  int convertStrToFilter(char* str);
  int convertStrToWrap(char* str);
  int convertStrToTexEnv(char* str);

public:
  TextureMgr();
  virtual ~TextureMgr();

  // loads texture name from a data file
  bool loadTextures(char* textureDataFile);

  // release all textures
  bool releaseTextures();

  // returns the texture's id for use in the texture manager
  unsigned int getTextureID(char* textureName);

  // binds the texture
  bool bind(unsigned int textureID);
};

#endif // __CHESS_TEXTURE_MANAGE_H__