#define FONT_MODULE 1
#include "common.h"

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

Font::Font()
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

