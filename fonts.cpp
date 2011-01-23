#define FONT_MODULE 1
#include "common.h"

#define INITIAL_CHARACTER '!'
#define SHADE_THRESHOLD 10

// TEMP:
//#define DEBUG

// Private method to find the shade of a particular pixel
int getPixelShade(SDL_Surface *surface, int x, int y)
{
	Uint8	*bits;
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

bool isShadeLight(int shade)
{
	return (shade != -1) && (shade >= SHADE_THRESHOLD);
}

bool isShadeDark(int shade)
{
	return (shade != -1) && (shade < SHADE_THRESHOLD);
}

/*Font::Font()
{
	// TODO: Fill this in.
}

inline float Font::getCharacterWidth(const char &c, const float &lineHeight) const
{
	if (c < MIN_FONT_CODE || c > max_char_available)
		return 0.0f;
	else
	{
		int index = c - MIN_FONT_CODE;
		return lineHeight * (char_uvs[index].right - char_uvs[index].left) / (char_uvs[index].bottom - char_uvs[index].top);
	}
}

float Font::drawCharacter(const char& c, const float &lineHeight, const float &x, const float &y) const
{
	if (c < MIN_FONT_CODE || c > max_char_available)
		return 0;
	
	int index = c - MIN_FONT_CODE;
	float width = getCharacterWidth(c, lineHeight);
	
	glBegin(GL_QUADS);
		glTexCoord2f(char_uvs[index].left, char_uvs[index].top);
		glVertex2f(x, y);
		glTexCoord2f(char_uvs[index].right, char_uvs[index].top);
		glVertex2f(x + width, y);
		glTexCoord2f(char_uvs[index].right, char_uvs[index].bottom);
		glVertex2f(x + width, y + lineHeight);
		glTexCoord2f(char_uvs[index].left, char_uvs[index].bottom);
		glVertex2f(x, y + lineHeight);
	glEnd();
	
	return width;
}

void Font::drawCenteredString(const string &s, const float &lineHeight, const float &x, const float &y, const float &width, const float &height, const FontAlignment &align) const
{
	float max_width = 0;
	vector<pair<string,float> > lines;
	unsigned int i = 0, next;
	float l_width;
	
	while (i < s.length() && i != string::npos)
	{
		next = s.find("\n", i+1);
		string sub = s.substr(i, next);
		l_width = 0;
		for (unsigned int j=0; j < sub.length(); j++)
			l_width += getCharacterWidth(sub.at(j), height);
		if (l_width > max_width)
			max_width = l_width;
		lines.push_back(pair<string,float>(sub, l_width));
	}
	
	float offset_x = 0.5f*(width - max_width);
	float offset_y = 0.5f*(height - lineHeight*lines.size());
	
	for (i=0; i < lines.size(); i++)
	{
		glPushMatrix();
		if (align == LEFT)
			glTranslatef(offset_x, offset_y + i*lineHeight, 0);
		else if (align == RIGHT)
			glTranslatef(width - lines.at(i).second, offset_y + i*lineHeight, 0);
		else if (align == CENTER)
			glTranslatef(width - offset_x - lines.at(i).second, offset_y + i*lineHeight, 0);
		
		for (unsigned int j=0; j < lines.at(i).first.size(); j++)
			glTranslatef(drawCharacter(lines.at(i).first.at(j), lineHeight, x, y), 0, 0);
		
		glPopMatrix();
	}
} // end drawCenteredString()

float Font::getStringWidth(const string &s, const float &lineHeight) const
{
	float max_width = 0, width = 0;
	
	for (unsigned int i=0; i < s.length(); i++)
	{
		if (s.at(i) == '\n')
		{
			if (width > max_width)
				max_width = width;
			width = 0;
		}
		else
		{
			width += getCharacterWidth(s.at(i), lineHeight);
		}
	}
	
	return max_width;
}

float Font::getStringHeight(const string &s, const float &lineHeight) const
{
	int num_rows = 0;
	
	for (unsigned int i=0; i < s.length(); i++)
		if (s.at(i) == '\n')
			num_rows ++;
	
	return num_rows * lineHeight;
}
*/

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
	if (isShadeDark(getPixelShade(origin, 0, 0)))
	{
		while (isShadeDark(getPixelShade(origin, cur_x++, 0))) ;
	}
	
	cur_char = INITIAL_CHARACTER;
	// Load in all the characters
	while (cur_char < NUM_FONT_CHARACTERS && cur_x < origin->w)
	{
		// Find the width of this character
		for (width=0; (cur_x + width) < origin->w; width++)
		{
			if (!isShadeDark(getPixelShade(origin, cur_x+width, 0)))
				break;
		}
		//for (width=0; getPixelShade(origin, cur_x + width, 0) <= SHADE_THRESHOLD; width ++) ;
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
				if (isShadeLight(getPixelShade(origin, cur_x + x, y)))
				{
					font->bitmaps[cur_char][byte_width*row + (x / 8)] |= (1 << (x % 8));
				}
			}
		}
		
#ifdef DEBUG
		printf("\n %c: Width(%d), Start(%d)", cur_char, width, cur_x); fflush(stdout);
#endif
		
		// Find the next character
		cur_x += width;
/*		while (true)
		{
			if (cur_x >= origin->w)
			{
				cur_x += 5;
				break;
			}
			if (!isShadeDark(getPixelShade(origin, cur_x, 0)))
				break;
			cur_x ++;
		}*/
		for (; (cur_x <= origin->w) && (getPixelShade(origin, cur_x, 0) < SHADE_THRESHOLD); cur_x ++) ;
		
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
		
		printf("Skipped a character '%c'\n", c);
		
		return;
	}
	
	printf("Drawing a letter '%c'...", c); fflush(stdout);
	glBitmap(font->widths[index], font->height, 0, 0, font->widths[index], 0, font->bitmaps[index]);
	printf("Done.\n");
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

