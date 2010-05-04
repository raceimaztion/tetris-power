/* ********************* *
 * Font-rendering module *
 * ********************* */

#ifndef FONTS_HEADER
#define FONTS_HEADER

// The minimum character we store
#define MIN_FONT_CODE '!'

// The maximum number of characters we store (working from '!' to 127)
#define NUM_FONT_CHARACTERS (128 - MIN_FONT_CODE)

// The maximum number of rows we support
#define NUM_FONT_ROWS 16

typedef struct {
	float left, right, top, bottom; // The UV coordinates for each side of the character
} CharacterExtents;

enum FontAlignment { LEFT, RIGHT, CENTER };

class Font {
  private:
    CharacterExtents char_uvs[NUM_FONT_CHARACTERS]; // THe UV coordinates for all characters
    unsigned char max_char_available;           // The maximum character available
    
  public:
    Font(); // TODO: Fill this in
    
    /**
     * Returns the width of a character given the character code and the desired height.
     * Returns zero if the character is unprintable.
     */
    float getCharacterWidth(const char &c, const float &lineHeight) const;
    
    /**
     * Draws a character with the top left corner at the specified coordinates.
     * Returns the width of the given character.
     */
    float drawCharacter(const char &c, const float &lineHeight, const float &x, const float &y) const;
    
    /**
     * Draws a string (correctly interpreting linebreaks) with the given lineHeight, boxed into the box
     *   specified by x,y,width,height.
     * s: The string to draw.
     * lineHeight: The height of each line, in unscaled units.
     * x,y: The coordinate of the top-left corner of the bounding box.
     * width,height: The width and height of the bounding box.
     *               (note that if not specified or zero, the box will be centered on x,y)
     */
    void drawCenteredString(const string &s, const float &lineHeight, const float &x, const float &y, const float &width=0.0f, const float &height=0.0f, const FontAlignment &align=LEFT) const;
    
    /**
     * Returns the width of the given string in units.
     */
    float getStringWidth(const string &s, const float &lineHeight) const;
    
    /**
     * Returns the height of the given string in units.
     */
    float getStringHeight(const string &s, const float &lineHeight) const;
};

// *******************************
// Old Fonts module:
// *******************************
/*

// This represents a single bitmapped font
typedef struct {
  unsigned char *bitmaps[NUM_FONT_CHARACTERS];  // Pointers the bitmaps
  unsigned char widths[NUM_FONT_CHARACTERS];    // The width of each character
  unsigned short height;          // The height of all characters
} Font;

/ *
  Parses a font out of an SDL_Surface.
  Requires the surface to be 8-bit grayscale.
  After parsing, the surface can be unloaded.
* /
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

// Load a Font
Font* fLoadFont(const string& fileName);
*/
#endif

