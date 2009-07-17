#include "common.h"

Texture::Texture()
{
  textureNumber = 0;
  valid = false;
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
  GLuint index;
  textureNumber = 2;
  glGenTextures(1, &index);
  textureNumber = index;
  printf("Texture index: %d.\n", index);
  
  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, textureNumber);
  
  // Send the texture to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, numColours, surface->w, surface->h,
               0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
  
  if (glGetError() != GL_NO_ERROR)
    printf("Texture::Texture(SDL_Surface*): An OpenGL errro has occurred!\n");
  
  // Set the stretching mode
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  if (glIsTexture(textureNumber))
    printf("Texture::Texture(SDL_Surface*): OpenGL texture seems valid.\n");
  else
    printf("Texture::Texture(SDL_Surface*): OpenGL texture seems invalid.\n");
  
  valid = true;
}

Texture::Texture(const Texture& t)
{
  textureNumber = t.textureNumber;
  valid = t.valid;
}

Texture::~Texture()
{
  glDeleteTextures(1, &textureNumber);
}

void Texture::applyTexture() const
{
//  if (!valid) return;
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureNumber);
}

void Texture::cancelTextures()
{
//  glDisable(GL_TEXTURE_2D);
}

bool Texture::isValid() const
{
  return valid;
}

GLuint Texture::getTextureIndex()
{
  return textureNumber;
}

