#include "common.h"

#define _CURVE_BACKGROUND_THICKNESS 0.75f

Texture::Texture()
{
  textureNumber = 0;
  valid = false;
}

Texture::Texture(int texNum)
{
  textureNumber = texNum;
  valid = glIsTexture(textureNumber);
  
  if (valid)
    printf("Texture::Texture(int): OpenGL texture seems valid.\n");
  else
    printf("Texture::Texture(int): OpenGL texture seems invalid.\n");
}

Texture::Texture(SDL_Surface* surface)
{
  if (surface == NULL)
  {
    printf("Runtime warning: SDL Surface passed is NULL!\n");
    textureNumber = 0;
    valid = false;
    return;
  }
  
  printf("Loading image...\n");
  
  GLenum textureFormat;
  GLint numColours;
  
  // Check if the texture is a power of 2 wide:
  if ((surface->w & (surface->w-1)) != 0)
    printf("Runtime warning: Image is not a power of 2 wide. This may cause some rendering issues.\n");
  // Check if the texture is a power of 2 high:
  if ((surface->h & (surface->h-1)) != 0)
    printf("Runtime warning: Image is not a power of 2 high. This may cause some rendering issues.\n");
  
  // Find the number of channels
  numColours = surface->format->BytesPerPixel;
  printf("Image has %d colour channels.\n", numColours);
  if (numColours == 4) // Has an Alpha channel
  {
    if (surface->format->Rmask == 0x000000ff)
      textureFormat = GL_RGBA;
    else
      textureFormat = GL_BGRA;
  }
  else if (numColours == 3) // No Alpha channel
  {
    if (surface->format->Rmask == 0x000000ff)
      textureFormat = GL_RGB;
    else
      textureFormat = GL_BGR;
  }
  else
  {
    printf("Runtime warning: Image is not true-colour! Colour-mapped images not supported!\n");
    throw "Image not true-colour. Colour-mapped images not supported.";
  }
  
  // Get a texture handle
  GLuint index = 0;
  textureNumber = 0;
  printf("Asking for texture number...\n");
  glGenTextures(1, &index);
  textureNumber = index;
  printf("Texture index: %d.\n", textureNumber);
  
  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, textureNumber);
  
  // Set the stretching mode
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  // Texture-colour mode
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  // Send the texture to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, numColours, surface->w, surface->h,
               0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
  gluBuild2DMipmaps(GL_TEXTURE_2D, numColours, surface->w, surface->h,
               textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
  
  if (glGetError() != GL_NO_ERROR)
    printf("Texture::Texture(SDL_Surface*): An OpenGL error has occurred!\n");
  
#ifdef DEBUG
  if (glIsTexture(textureNumber))
    printf("Texture::Texture(SDL_Surface*): OpenGL texture seems valid.\n");
  else
    printf("Texture::Texture(SDL_Surface*): OpenGL texture seems invalid.\n");
#endif
  
  valid = true;
}

Texture::Texture(SDL_Surface* surface, GLenum format)
{
  if (surface == NULL)
  {
    printf("Runtime warning: SDL Surface passed is NULL!\n");
    textureNumber = 0;
    valid = false;
    return;
  }
  
  printf("Loading image...\n");
  
  GLenum textureFormat;
  GLint numColours;
  
  // Check if the texture is a power of 2 wide:
  if ((surface->w & (surface->w-1)) != 0)
    printf("Runtime warning: Image is not a power of 2 wide. This may cause some rendering issues.\n");
  // Check if the texture is a power of 2 high:
  if ((surface->h & (surface->h-1)) != 0)
    printf("Runtime warning: Image is not a power of 2 high. This may cause some rendering issues.\n");
  
  // Find the number of channels
  numColours = surface->format->BytesPerPixel;
  printf("Image has %d colour channels.\n", numColours);
  if (numColours == 4) // Has an Alpha channel
  {
    if (surface->format->Rmask == 0x000000ff)
      textureFormat = GL_RGBA;
    else
      textureFormat = GL_BGRA;
  }
  else if (numColours == 3) // No Alpha channel
  {
    if (surface->format->Rmask == 0x000000ff)
      textureFormat = GL_RGB;
    else
      textureFormat = GL_BGR;
  }
  else if (numColours == 2) // Greyscale with Alpha
    textureFormat = GL_LUMINANCE_ALPHA;
  else
  {
    printf("Runtime warning: Image is not true-colour! Colour-mapped images not supported!\n");
    throw "Image not true-colour. Colour-mapped images not supported.";
  }
  
  // Get a texture handle
  GLuint index = 0;
  textureNumber = 0;
  printf("Asking for texture number...\n");
  glGenTextures(1, &index);
  textureNumber = index;
  printf("Texture index: %d.\n", textureNumber);
  
  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, textureNumber);
  
  // Set the stretching mode
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  // Texture-colour mode
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  // Send the texture to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, numColours, surface->w, surface->h,
               0, format, GL_UNSIGNED_BYTE, surface->pixels);
  gluBuild2DMipmaps(GL_TEXTURE_2D, numColours, surface->w, surface->h,
               format, GL_UNSIGNED_BYTE, surface->pixels);
  
  if (glGetError() != GL_NO_ERROR)
    printf("Texture::Texture(SDL_Surface*): An OpenGL error has occurred!\n");
  
//#ifdef DEBUG
  if (glIsTexture(textureNumber))
    printf("Texture::Texture(SDL_Surface*): OpenGL texture seems valid.\n");
  else
    printf("Texture::Texture(SDL_Surface*): OpenGL texture seems invalid.\n");
//#endif
  
  valid = true;
}

Texture::Texture(const Texture& t)
{
  textureNumber = t.textureNumber;
  valid = t.valid;
}

Texture::~Texture()
{
//  if (valid) glDeleteTextures(1, &textureNumber);
}

void Texture::applyTexture() const
{
  if (!valid)
  {
    printf("Runtime warning: Texture::applyTexture(): Trying to apply an invalid texture.\n\t\tNot doing anything.\n");
    return;
  }
#ifdef DEBUG
  else
    printf("Applying texture number %d.\n", textureNumber);
#endif
  
  glBindTexture(GL_TEXTURE_2D, textureNumber);
  glEnable(GL_TEXTURE_2D);
}

void Texture::cancelTextures()
{
  glDisable(GL_TEXTURE_2D);
}

bool Texture::isValid() const
{
  return valid;
}

GLuint Texture::getTextureIndex()
{
  return textureNumber;
}

Texture texMakeCheckerboard()
{
  GLuint texNum;
  GLubyte image[64][64][3];
  int c;
  for (int i=0; i < 64; i++)
  {
    for (int j=0; j < 64; j++)
    {
      c = 255 * (((i&8) == 0) ^ ((j&8) == 0));
      image[i][j][0] = image[i][j][1] = image[i][j][2] = (GLubyte) c;
    }
  }
  
  glGenTextures(1, &texNum);
  printf("Checkerboard texture number is %d.\n", texNum);
  
  glBindTexture(GL_TEXTURE_2D, texNum);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, image);
  
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  
  return Texture(texNum);
}

Texture texMakeCurveBorder(int width, int height, bool alpha)
{
  GLubyte image[width*height*2], b;
  float fx, fy, sample;
  for (int x=0; x < width; x++)
  {
    fx = (float)x/(width-1);
    for (int y=0; y < height; y++)
    {
      fy = (float)y/(height-1);
      sample = sqrtf(fx*fx + fy*fy);
      if (sample > 0.5f)
        sample = 1.0f - sample;
      b = (GLubyte)(255*max(0.0f, min(1.0f, 2.0f*sample)));
      if (alpha)
      {
        image[2*(x+y*width)] = 255;
        image[2*(x+y*width)+1] = b;
      }
      else
        image[x+y*width] = b;
    }
  }
  
  GLuint index;
  printf("Asking for texture.\n");
  glGenTextures(1, &index);
  
  glBindTexture(GL_TEXTURE_2D, index);
  
  if (alpha)
    glTexImage2D(GL_TEXTURE_2D, 0, 2, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, image);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, 2, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  if (glIsTexture(index))
    printf("Created curve-border texture with index %d and it seems valid.\n", index);
  else
    printf("Created curve-border texture with index %d and it doesn't seem valid. Attempting to use it anyway.\n", index);
  return Texture(index);
}

Texture texMakeCurveBackground(int width, int height, bool alpha)
{
  GLubyte image[width*height*2], b;
  float fx, fy, sample;
  for (int x=0; x < width; x++)
  {
    fx = (float)x/(width-1);
    for (int y=0; y < height; y++)
    {
      fy = (float)y/(height-1);
      sample = sqrtf(fx*fx + fy*fy);
      sample = 1.0f - (sample - 0.5f)/_CURVE_BACKGROUND_THICKNESS;
      b = (GLubyte)(255*max(0.0f, min(1.0f, sample)));
      if (alpha)
      {
        image[2*(x+y*width)] = 255;
        image[2*(x+y*width)+1] = b;
      }
      else
        image[x+y*width] = b;
    }
  }
  
  GLuint index;
  printf("Asking for texture.\n");
  glGenTextures(1, &index);
  
  glBindTexture(GL_TEXTURE_2D, index);
  
  if (alpha)
    glTexImage2D(GL_TEXTURE_2D, 0, 2, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, image);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, 2, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  if (glIsTexture(index))
    printf("Created curve-background texture with index %d and it seems valid.\n", index);
  else
    printf("Created curve-background texture with index %d and it doesn't seem valid. Attempting to use it anyway.\n", index);
  return Texture(index);
}

Texture texLoadTexture(const string& fileName)
{
  SDL_Surface *surface = IMG_Load(fileName.c_str());
  if (surface == NULL)
  {
    printf("Runtime warning: Failed to load texture from file '%s'\n", fileName.c_str());
    return Texture();
  }
  Texture tex(surface);
  SDL_FreeSurface(surface);
  return tex;
}

Texture texLoadCustomTexture(const string& fileName, GLenum format)
{
  SDL_Surface *surface = IMG_Load(fileName.c_str());
  if (surface == NULL)
  {
    printf("Runtime warning: Failed to load texture from file '%s'\n", fileName.c_str());
    return Texture();
  }
  Texture tex(surface, format);
  SDL_FreeSurface(surface);
  return tex;
}

