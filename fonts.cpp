#define FONT_MODULE 1
#include "common.h"

#define INITIAL_CHARACTER '!'
#define SHADE_THRESHOLD 10

// TEMP:
//#define DEBUG

// Private method to find the shade of a particular pixel
int getPixelShade(SDL_Surface *surface, int x, int y)
{
  Uint8  *bits;
  Uint32 bpp;
  
  if (x < 0 || y < 0 ||
    x >= surface->w ||
    y >= surface->h)
   return -1;
  
  bpp = surface->format->BytesPerPixel;
  bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;

  // Get the pixel
  switch(bpp) {
    case 1:
      return *((Uint8 *)surface->pixels + y * surface->pitch + x);
    
    case 2:
      return (*((Uint16 *)surface->pixels + y * surface->pitch/2 + x)) >> 8;
    
    case 3: // Format/endian independent
    case 4:
      {
        int r, g, b;
        r = *((bits)+surface->format->Rshift/8);
        g = *((bits)+surface->format->Gshift/8);
        b = *((bits)+surface->format->Bshift/8);
        
        // NOTE: I suppose that could use "proper" greyscale shading here, but then I'd be dealing with floats...
        return (r + g + b)/3;
      }
  }

  return -1;
}

Font *fParseFont(SDL_Surface* origin)
{
  Font* font = (Font *)calloc(sizeof(Font), 1);
  int cur_x = 0, cur_char, width, byte_width;
  
  if (origin == NULL)
    return NULL;
  
  printf("Information: Starting to parse font with %d bytes per pixel...", origin->format->BytesPerPixel);
  fflush(stdout);
  
  for (unsigned int i=0; i < NUM_FONT_CHARACTERS; i++)
    font->bitmaps[i] = NULL;
  
  // Since we're not saving the first row of pixels
  font->height = origin->h - 1;
  
  // Fill the first few bitmaps with NULLs and widths of four pixels
  for (unsigned int i=0; i < INITIAL_CHARACTER; i++)
  {
    font->bitmaps[i] = NULL;
    font->widths[i] = 4;
  }
  
  // If the first character is not at the beginning of the image, skip the initial part
  if (getPixelShade(origin, 0, 0) > SHADE_THRESHOLD)
  {
    while (getPixelShade(origin, cur_x++, 0) > SHADE_THRESHOLD) ;
  }
  
  cur_char = INITIAL_CHARACTER;
  // Load in all the characters
  while (cur_char < NUM_FONT_CHARACTERS && cur_x < origin->w)
  {
    // Find the width of this character
    for (width=0; getPixelShade(origin, cur_x + width, 0) <= SHADE_THRESHOLD; width ++) ;
    if (width < 1) width = 1;
    
    // The width of the bitmap in bytes
    byte_width = width / 8;
    if ((byte_width < 1) || ((width % 8) != 0)) byte_width++;
    
    // Allocate the memory for this character's bitmap
    font->bitmaps[cur_char] = (unsigned char *)calloc(byte_width, font->height);
    if (font->bitmaps[cur_char] == NULL)
    {
      fFreeFont(font);
      printf(" Error: Failed to init font!\n");
      return NULL;
    }
    font->widths[cur_char] = width;
    
    // Copy the character pixel by pixel into its bitmap
    for (int y=font->height,row=0; y > 0; y--,row++)
    {
      for (int x=0; x < width; x++)
      {
        if (getPixelShade(origin, cur_x + x, y) >= SHADE_THRESHOLD)
        {
          font->bitmaps[cur_char][byte_width*row + (x / 8)] |= (1 << (x % 8));
        }
      }
    }
    
#ifdef DEBUG
    printf("%c: %2d, %d, ", cur_char, width, byte_width); fflush(stdout);
#endif
    
    // Find the next character
    cur_x += width;
    for (; (cur_x <= origin->w) && (getPixelShade(origin, cur_x, 0) >= SHADE_THRESHOLD); cur_x ++) ;
    
    cur_char ++;
  }
  
  printf("Done parsing %d characters.\n", cur_char);
  
  return font;
}

void fFreeFont(Font* font)
{
  if (font == NULL)
    return;
  
  for (int i=0; i < NUM_FONT_CHARACTERS; i++)
    if (font->bitmaps[i] != NULL)
      free(font->bitmaps[i]);
}

void fDrawLetter(Font* font, const char c)
{
  // Do we have a font?
  if (font == NULL)
    return;
  
  int index = (int)c;
  
  // Is this a printable character?
  if (font->bitmaps[index] == NULL)
  {
    // Not printable, just move the raster position over a couple pixels
    
    glBitmap(0, 0, 0, 0, font->widths[index], 0, NULL);
    
    return;
  }
  
  glBitmap(font->widths[index], font->height, 0, 0, font->widths[index], 0, font->bitmaps[index]);
}

void fDrawString(Font* font, const char* c)
{
  if (font == NULL)
  {
    printf("Warning: No font to draw any text! Not doing anything\n");
    return;
  }
  
  for (int i=0; c[i] != 0; i++)
    fDrawLetter(font, c[i]);
}

int fLetterWidth(Font* font, const char c)
{
  if (font == NULL)
    return 0;
  else
    return font->widths[(int)c];
}

int fStringWidth(Font* font, const char* c)
{
  if (font == NULL)
    return 0;
  
  int result = 0;
  for (int i=0; c[i] != 0; i++)
    result += fLetterWidth(font, c[i]);
  
  return result;
}

int fFontHeight(Font *font)
{
  if (font == NULL)
    return 0;
  else
    return font->height;
}

Font *fLoadFont(const string& fileName)
{
  SDL_Surface *fontSurface = IMG_Load(fileName.c_str());
  if (fontSurface == NULL)
  {
    fprintf(stderr, "Warning: Failed to load font from file '%s', you might not get some text in-game!\n", fileName.c_str());
    return NULL;
  }
  Font *font = fParseFont(fontSurface);
  SDL_FreeSurface(fontSurface);
  if (font == NULL)
  {
    fprintf(stderr, "Warning: Failed to parse font from file '%s', you might not get some text in-game!\n", fileName.c_str());
    return NULL;
  }
  return font;
}

