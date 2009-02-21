#define COMMON_MODULE
#include "common.h"

/* ************ *
 * Colour class *
 * ************ */
Colour::Colour()
{
  r = g = b = 0.0f;
  a = 1.0f;
}

Colour::Colour(float shade)
{
  r = g = b = shade;
  a = 1.0f;
}

Colour::Colour(float shade, float alpha)
{
  r = g = b = shade;
  a = alpha;
}

Colour::Colour(float red, float green, float blue)
{
  r = red;
  g = green;
  b = blue;
  a = 1.0f;
}

Colour::Colour(float red, float green, float blue, float alpha)
{
  r = red;
  g = green;
  b = blue;
  a = alpha;
}

void Colour::fillArray(float a[])
{
  a[0] = r;
  a[1] = g;
  a[2] = b;
  a[3] = this->a;
}

void Colour::apply()
{
  glColor4f(r, g, b, a);
}

void Colour::applyMaterial()
{
  float values[4] = {r, g, b, a};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, values);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, values);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, values);
}

void Colour::applyMaterial(int attribute)
{
  float values[4];
  values[0] = r;
  values[1] = g;
  values[2] = b;
  values[3] = a;
  glMaterialfv(GL_FRONT_AND_BACK, attribute, values);
}

float Colour::brightness()
{
  return sqrtf(r*r + g*g + b*b);
}

/* ************** *
 * Position class *
 * ************** */
Position::Position()
{
  x = y = z = 0.0f;
}

Position::Position(const Position& p)
{
  x = p.x;
  y = p.y;
  z = p.z;
}

Position::Position(float x, float y, float z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}

void Position::applyTranslation()
{
  glTranslatef(x, y, z);
}

void Position::applyTranslation(float amount)
{
  glTranslatef(amount*x, amount*y, amount*z);
}

void Position::applyNormal()
{
  glNormal3f(x, y, z);
}

void Position::applyVertex()
{
  glVertex3f(x, y, z);
}

void Position::applyTexCoords()
{
  glTexCoord2f(x, y);
}

Position Position::operator+=(const Position& p)
{
  x += p.x;
  y += p.y;
  z += p.z;
  return *this;
}

/* *********** *
 * Light class *
 * *********** */
Light::Light(const Position& p, float w, const Colour& c)
{
  pos = p;
  this->w = w;
  this->c = c;
}

void Light::apply(int number)
{
  glEnable(GL_LIGHTING);
  glEnable(number);
  float values[] = {pos.x, pos.y, pos.z, w};
  glLightfv(number, GL_POSITION, values);
  c.fillArray(values);
  glLightfv(number, GL_AMBIENT, values);
  glLightfv(number, GL_DIFFUSE, values);
  glLightfv(number, GL_SPECULAR, values);
}

void Light::disable(int number)
{
  glDisable(number);
}

/* ******************************** *
 * Screen class and related methods *
 * ******************************** */
Screen::Screen()
{
  needRepaint = false;
  visible = false;
}

Screen::~Screen()
{
}

void Screen::end()
{
  screenPop();
}

bool Screen::replaceWith(const int screenID)
{
  screenPop();
  return call(screenID);
}

bool Screen::call(const int screenID)
{
  return screenActivate(screenID);
}

void Screen::markRepaint()
{
  needRepaint = true;
}

void Screen::clearRepaint()
{
  needRepaint = false;
}

bool Screen::needsRepaint()
{
  return needRepaint;
}

void Screen::setVisible(bool visible)
{
  this->visible = visible;
}

bool Screen::isVisible()
{
  return visible;
}

int Screen::getWidth()
{
  return win_width;
}

int Screen::getHeight()
{
  return win_height;
}

// Screen class stubs:
void Screen::timerTick() { }
void Screen::prepareForShow() { }
void Screen::prepareForHide() { }
void Screen::screenPaint() { }
void Screen::keyboard(const SDL_keysym &key) { }
void Screen::mouseButton(const SDL_MouseButtonEvent &mouse) { }
void Screen::mouseMotion(const SDL_MouseMotionEvent &mouse) { }
bool Screen::isOpaque() { return true; }


// Screen-related variables
map<int,Screen*> screenMap;
vector<Screen*> screenStack;

// Screen-related methods

bool screenAddNew(Screen &next, int screenID)
{
  if (screenMap.find(screenID) != screenMap.end())
    return false;
  
  screenMap[screenID] = &next;
  
  return true;
}

bool screenActivate(int screenID)
{
  if (screenMap.find(screenID) == screenMap.end())
    return false;
  
  if (screenMap[screenID]->isVisible())
    return false;
  
  if (!screenStack.empty())
  {
    screenStack.back()->setVisible(false);
    screenStack.back()->prepareForHide();
  }
  
  screenStack.push_back(screenMap[screenID]);
  screenStack.back()->setVisible(true);
  screenStack.back()->prepareForShow();
  
  return true;
}

void screenPop()
{
  if (screenStack.empty())
    return;
  
  screenStack.back()->setVisible(false);
  screenStack.back()->prepareForHide();
  screenStack.pop_back();
  
  if (!screenStack.empty())
  {
    screenStack.back()->setVisible(true);
    screenStack.back()->prepareForShow();
  }
}

bool screenTimerTick()
{
  if (!screenStack.empty())
  {
    screenStack.back()->timerTick();
    return screenStack.back()->needsRepaint();
  }
  
  return false;
}

void _screenRecursivePaint(int cur)
{
  if ((cur < 0) || (screenStack.at(cur)->isOpaque()))
    return;
  
  _screenRecursivePaint(cur - 1);
  
  screenStack.at(cur)->screenPaint();
  screenStack.at(cur)->clearRepaint();
}

void screenPaint()
{
  if (screenStack.empty())
    return;
  else
    _screenRecursivePaint(screenStack.size() - 1);
}

void screenKeyboard(const SDL_keysym &key)
{
  if (!screenStack.empty())
    screenStack.back()->keyboard(key);
}

void screenMouseButton(const SDL_MouseButtonEvent &mouse)
{
  if (!screenStack.empty())
    screenStack.back()->mouseButton(mouse);
}

void screenMouseMotion(const SDL_MouseMotionEvent &mouse)
{
  if (!screenStack.empty())
    screenStack.back()->mouseMotion(mouse);
}

/* ******************************* *
 * Some generally-useful functions *
 * ******************************* */
// Colour-related
Colour operator*(float s, const Colour& c)
{
  return Colour(s*c.r, s*c.g, s*c.b, c.a);
}

Colour operator*(const Colour& c, float s)
{
  return Colour(s*c.r, s*c.g, s*c.b, c.a);
}

// Position-related
Position operator+(const Position& a, const Position& b)
{
  return Position(a.x+b.x, a.y+b.y, a.z+b.z);
}

Position operator-(const Position& a, const Position& b)
{
  return Position(a.x-b.x, a.y-b.y, a.z-b.z);
}

Position operator*(const Position& p, float s)
{
  return Position(s*p.x, s*p.y, s*p.z);
}

Position operator*(float s, const Position& p)
{
  return Position(s*p.x, s*p.y, s*p.z);
}

Position operator/(const Position& p, float s)
{
  return Position(p.x/s, p.y/s, p.z/s);
}

// Drawing-related
void comDrawCube(float x, float y, float scale, float rotation)
{
  glPushMatrix();
  
  
  
  glPopMatrix();
}

void comDrawSphere(float x, float y, float scale, float rotation)
{
  glPushMatrix();
  
  
  
  glPopMatrix();
}

void comDrawBlock(float x, float y, float scale, float rotation)
{
  comDrawCube(x, y, scale, rotation);
}

// Random-related
inline float comRandomFloat()
{
  return (float)rand() / RAND_MAX;
}

inline float comRandomBoundedFloat(float min, float max)
{
  return (max - min)*comRandomFloat() + min;
}

Colour comRandomColour()
{
  Colour result(comRandomBoundedFloat(0.125f, 0.875f),
                comRandomBoundedFloat(0.125f, 0.875f),
                comRandomBoundedFloat(0.125f, 0.875f));
  
  float brightness = comRandomBoundedFloat(0.25f, 0.75f);
  brightness /= result.brightness();
  result = result * brightness;
  
  return result;
}

// File-related
string comReadLine(FILE* in)
{
  string result;
  
  if (in == NULL)
    return result;
  
  char c = getc(in);
  while (!feof(in) && c != '\n')
  {
    result += c;
    c = getc(in);
  }
  
  return result;
}

// String-related
bool comStartsWith(string& a, string& b)
{
  return a.find_first_of(b) == 0;
}

vector<string> comSplitSpaces(string a)
{
  vector<string> result;
  
  unsigned int last = 0, next = a.find_first_of(" ");
  
  do
  {
    next = a.find_first_of(" ", last+1);
    if (next == string::npos)
    {
      result.push_back(a.substr(last));
      break;
    }
    result.push_back(a.substr(last, next-last));
    last = next;
  }
  while (next != string::npos);
  
  vector<string>::iterator iter = result.begin();
  while (iter != result.end())
  {
    while ((*iter).size() > 0 && (*iter)[0] == ' ')
      (*iter) = (*iter).substr(1);
    
    if ((*iter).size() < 1)
      result.erase(iter);
    else
      iter++;
  }
  
  return result;
}

vector<string> comSplitString(string& a, const string& delim)
{
  vector<string> result;
  unsigned int last = 0, next = a.find_first_of(delim);
  
  do
  {
    next = a.find_first_of(delim, last+delim.size());
    if (next == string::npos)
    {
      result.push_back(a.substr(last));
      break;
    }
    result.push_back(a.substr(last, next-last));
    last = next;
  }
  while (next != string::npos);
  
  return result;
}

