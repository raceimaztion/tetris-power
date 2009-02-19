/* ********************* *
 * Font-rendering module *
 * ********************* */

// The maximum number of characters we store
#define NUM_FONT_CHARACTERS 128

// This represents a single bitmapped font
typedef struct {
  unsigned char *bitmaps[NUM_FONT_CHARACTERS];  // Pointers the bitmaps
  unsigned char widths[NUM_FONT_CHARACTERS];    // The width of each character
  unsigned short height;          // The height of all characters
} Font;

/*
  Parses a font out of an SDL_Surface.
  Requires the surface to be 8-bit grayscale.
  After parsing, the surface can be unloaded.
*/
Font* fParseFont(SDL_Surface* origin);
// Frees the font after use
void fFreeFont(Font *font);

// Draws a single letter or a string at the current OpenGL raster position
void fDrawLetter(Font *font, const char c);
void fDrawString(Font *font, const char *s);

// Get the width of a letter or string
int fLetterWidth(Font *font, const char c);
int fStringWidth(Font *font, const char *s);

// Get a font's height:
int fFontHeight(Font *font);

