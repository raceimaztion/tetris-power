#define COMMON_MODULE
#include "common.h"

// Variables private to the Common module
Mesh cube;
Texture cubeTex;

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

void Colour::fillArray(float a[]) const
{
  a[0] = r;
  a[1] = g;
  a[2] = b;
  a[3] = this->a;
}

void Colour::apply() const
{
  glColor4f(r, g, b, a);
}

void Colour::apply(float alpha) const
{
  glColor4f(r, g, b, alpha);
}

void Colour::applyMaterial() const
{
  float values[4] = {r, g, b, a};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, values);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, values);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, values);
}

void Colour::applyMaterial(int attribute) const
{
  float values[4] = {r, g, b, a};
  glMaterialfv(GL_FRONT_AND_BACK, attribute, values);
}

void Colour::applyLightAttribute(int number, int attribute) const
{
  float values[4] = {r, g, b, a};
  glLightfv(number, attribute, values);
}

void Colour::applyLightAttribute(int number, int attribute, float power) const
{
  float values[4] = {r*power, g*power, b*power, a};
  glLightfv(number, attribute, values);
}

float Colour::brightness() const
{
  return sqrtf(r*r + g*g + b*b);
}

Colour Colour::brighter(float v) const
{
  // nc = (1-(1-c)*(1-v)) = 1 - ((1-v) - (1-v)*c) = 1 - (1 - v - c + c*v) = v + c - c*v
  return Colour(v+r-v*r, v+g-v*g, v+b-v*b, a);
}

Colour Colour::darker(float v) const
{
  return Colour(r*(1-v), g*(1-v), b*(1-v), a);
}

string Colour::toString() const
{
  ostringstream result;
  result.width(6);
  result << "Colour: " << r << ", " << g << ", " << b;
  
  return result.str();
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

void Position::applyTranslation() const
{
  glTranslatef(x, y, z);
}

void Position::applyTranslation(float amount) const
{
  glTranslatef(amount*x, amount*y, amount*z);
}

void Position::applyNormal() const
{
  glNormal3f(x, y, z);
}

void Position::applyVertex() const
{
  glVertex3f(x, y, z);
}

void Position::applyTexCoords() const
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

void Light::apply(int number) const
{
  glEnable(GL_LIGHTING);
  glEnable(number);
  float values[] = {pos.x, pos.y, pos.z, w};
  glLightfv(number, GL_POSITION, values);
  c.applyLightAttribute(number, GL_AMBIENT);
  c.applyLightAttribute(number, GL_DIFFUSE);
  c.applyLightAttribute(number, GL_SPECULAR);
}

void Light::disable(int number) const
{
  glDisable(number);
}

/* ************ *
 * Camera class *
 * ************ */
Camera::Camera()
{
  Camera(Position(0, -5, 0), Position(0, 0, 0));
}

Camera::Camera(Position pos)
{
  Camera(pos, Position(0, 0, 0));
}

Camera::Camera(Position pos, Position rot)
{
  this->pos = pos;
  this->rotation = rot;
}

Camera::Camera(const Camera& c)
{
  pos = c.pos;
  rotation = c.rotation;
}

void Camera::apply() const
{
  gluLookAt(0, 0, 0,  // Start at 0,0,0
            0, 1, 0,  // Look at 0,1,0
            0, 0, 1); // Up is 0,0,1
  
  pos.applyTranslation();
//  glRotatef(rotation.z, 0, 0, 1);
//  glRotatef(rotation.y, 0, 1, 0);
//  glRotatef(rotation.x, 1, 0, 0);
}

void Camera::move(float dx, float dy, float dz)
{
  pos.x += dx;
  pos.y += dy;
  pos.z += dz;
}

void Camera::move(Position dp)
{
  pos += dp;
}

void Camera::rotate(float dx, float dy, float dz)
{
  rotation.x += dx;
  rotation.y += dy;
  rotation.z += dz;
}

void Camera::rotate(Position dr)
{
  rotation += dr;
}

void Camera::animate(float dTime)
{
  // TODO: Fill this in
}

/* ****************** *
 * Bezier curve class *
 * ****************** */
Bezier::Bezier()
{
  a = b = c = d = 0.0f;
  type = 0;
}

Bezier::Bezier(const Bezier& curve)
{
  a = curve.a;
  b = curve.b;
  c = curve.c;
  d = curve.d;
  type = curve.type;
}

Bezier::Bezier(float startV, float endV, float dStart, float dEnd)
{
  a = startV;
  b = dStart;
  c = 3*(endV - startV) - (dEnd + 2*dStart);
  d = 2*(startV - endV) + (dStart + dEnd);
  
  type = (a == 0.0f ? 0 : 0x01) |
         (b == 0.0f ? 0 : 0x02) |
         (c == 0.0f ? 0 : 0x04) |
         (d == 0.0f ? 0 : 0x08);
}

float Bezier::f(float t) const
{
  // TODO: Use 'type' to choose an equation to use for a slight speed-up
  //return a + t*(b + t*(c + t*d));
  switch (type)
  {
    case 0:
      return 0.0f;
    case 1:
      return a;
    case 2:
      return b*t;
    case 3:
      return a + b*t;
    case 4:
      return c*t*t;
    case 5:
      return a + c*t*t;
    case 6:
      return b*t + c*t*t;
    case 7:
      return a + b*t + c*t*t;
    case 8:
      return d*t*t*t;
    case 9:
      return a + d*t*t*t;
    case 10:
      return b*t + d*t*t*t;
    case 11:
      return a + b*t + d*t*t*t;
    case 12:
      return c*t*t + d*t*t*t;
    case 13:
      return a + c*t*t + d*t*t*t;
    case 14:
      return b*t + c*t*t + d*t*t*t;
    default:
      return a + b*t + c*t*t + d*t*t*t;
  }
}

float Bezier::df(float t) const
{
  return b + t*(2*c + 3*t*d);
}

bool Bezier::isFlat() const
{
  return type == 0;
}

void Bezier::changeCurve(float startV, float endV, float dStart, float dEnd)
{
  a = startV;
  b = dStart;
  c = 3*(endV - startV) - (dEnd + 2*dStart);
  d = 2*(startV - endV) + (dStart + dEnd);
  
  type = (a == 0.0f ? 0x01 : 0) |
         (b == 0.0f ? 0x02 : 0) |
         (c == 0.0f ? 0x04 : 0) |
         (d == 0.0f ? 0x08 : 0);
}

Bezier& Bezier::operator=(const Bezier& curve)
{
  a = curve.a;
  b = curve.b;
  c = curve.c;
  d = curve.d;
  type = curve.type;
  
  return *this;
}

/* ******************************** *
 * Screen class and related methods *
 * ******************************** */
Screen::Screen(int screenID)
{
  needRepaint = true;
  visible = false;
  this->screenID = screenID;
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
  if (screenExists(screenID))
  {
    screenPop();
    bool result = call(screenID);
    if (!result)
      screenActivate(getScreenID());
    return result;
  }
  else
  {
    fprintf(stderr, "Runtime warning: Failed to activate screen because we don't have a screen with id #%d.\n", screenID);
    return false;
  }
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

bool Screen::isVisible() const
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

int Screen::getScreenID() const
{
  return screenID;
}

// Screen class stubs:
void Screen::timerTick(float dTime) { }
void Screen::prepareForShow() { }
void Screen::prepareForHide() { }
void Screen::screenPaint() const { }
void Screen::keyboard(const SDL_KeyboardEvent &key) { }
void Screen::mouseButton(const SDL_MouseButtonEvent &mouse) { }
void Screen::mouseMotion(const SDL_MouseMotionEvent &mouse) { }
bool Screen::isOpaque() const { return true; }


// Screen-related variables
map<int,Screen*> screenMap;
vector<Screen*> screenStack;

// Screen-related methods

bool screenExists(int screenID)
{
  return screenMap.find(screenID) != screenMap.end();
}

bool screenAddNew(Screen &next, int screenID)
{
  if (screenExists(screenID))
  {
    fprintf(stderr, "Runtime warning: Failed to add a screen with id #%d.\n", screenID);
    return false;
  }
  
  screenMap[screenID] = &next;
  
  return true;
}

bool screenActivate(int screenID)
{
  if (!screenExists(screenID))
  {
    fprintf(stderr, "Runtime warning: Failed to activate screen because we don't have a screen with id #%d.\n", screenID);
    return false;
  }
  
  if (screenMap[screenID]->isVisible())
  {
    fprintf(stderr, "Runtime warning: Tried to activate an already-visible screen with id #%d.\n", screenID);
    return false;
  }
  
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
  {
    fprintf(stderr, "Runtime warning: Tried to pop a Screen of an empty Screen list.\n");
    return;
  }
  
  screenStack.back()->setVisible(false);
  screenStack.back()->prepareForHide();
  screenStack.pop_back();
  
  if (!screenStack.empty())
  {
    screenStack.back()->setVisible(true);
    screenStack.back()->prepareForShow();
  }
}

bool screenTimerTick(float dTime)
{
  if (!screenStack.empty())
  {
    screenStack.back()->timerTick(dTime);
    return screenNeedsRepaint();
  }
  
  return false;
}

void _screenRecursivePaint(int cur)
{
  if (cur < 0)
    return;
  
  if (!(screenStack.at(cur)->isOpaque()))
    _screenRecursivePaint(cur - 1);
  
  screenStack.at(cur)->clearRepaint();
  screenStack.at(cur)->screenPaint();
}

void screenPaint()
{
  screenPaint(false);
}

void screenPaint(bool force)
{
  if (screenStack.empty())
  {
    fprintf(stderr, "Runtime warning: No screens in stack to paint, doing nothing.\n");
    return;
  }
  else
  {
    if (!(force || screenNeedsRepaint()))
      return;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    _screenRecursivePaint(screenStack.size() - 1);
    SDL_GL_SwapBuffers();
  }
}

bool screenNeedsRepaint()
{
  for (int i=screenStack.size()-1; i >= 0; i++)
  {
    if (screenStack.at(i)->needsRepaint())
      return true;
    else if (screenStack.at(i)->isOpaque())
      return false;
  }
  return false;
}

void screenKeyboard(const SDL_KeyboardEvent &key)
{
  // Global key commands
  if (key.type == SDL_KEYDOWN && key.keysym.sym == SDLK_F11)
    toggleFullscreen();
  
  // Send events to the current screen
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

int screenNumStackedScreens()
{
  return screenStack.size();
}

/* ********************** *
 * Loading-system related *
 * ********************** */
Loadable::Loadable(string name)
{
  this->name = name;
  loadingFunction = NULL;
  done = false;
}

Loadable::Loadable(string name, void (*loadingFunction)())
{
  this->name = name;
  this->loadingFunction = loadingFunction;
  done = false;
}

string Loadable::getName() const
{
  return name;
}

bool Loadable::isDone() const
{
  return done;
}

void Loadable::loadItems()
{
  if (loadingFunction != NULL)
    loadingFunction();
  load();
}

void Loadable::load() { }

// Private variables fro the loader
vector<Loadable*> _loaderList;
unsigned int _loaderCurrentLoader = 0;
float _loaderPercentageDone = 0.0f;

// Add a loader class to the loading system
void loaderAddLoader(Loadable* loader)
{
  _loaderList.push_back(loader);
  
}

void loaderRunLoader()
{
  if (_loaderPercentageDone >= 1.0f)
    return;
  
  if (_loaderCurrentLoader >= _loaderList.size())
    return;
  
  _loaderList.at(_loaderCurrentLoader ++)->loadItems();
  _loaderPercentageDone = (float)_loaderCurrentLoader / _loaderList.size();
}

float loaderGetProgress()
{
  return _loaderPercentageDone;
}

bool loaderDoneLoading()
{
  return _loaderCurrentLoader >= _loaderList.size();
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
  
  glTranslatef(x + 0.5f, 0.0f, y + 0.5f);
  glScalef(scale, scale, scale);
  glRotatef(rotation, 0, 1, 0);
  cube.render();
  
  glPopMatrix();
}

void comDrawSphere(float x, float y, float scale, float rotation)
{
  glPushMatrix();
  
  glTranslatef(x + 0.5f, 0.0f, y + 0.5f);
  glScalef(scale, scale, scale);
  
  // TODO: Add code here to draw an appropriate sphere
  
  glPopMatrix();
}

void comDrawBlock(float x, float y, float scale, float rotation)
{
  comDrawCube(x, y, scale, rotation);
}

void comDrawTexturedCube(float x, float y, float scale, float rotation)
{
  glPushMatrix();
  
  glTranslatef(x + 0.5f, 0.0f, y + 0.5f);
  glScalef(scale, scale, scale);
  glRotatef(rotation, 0, 1, 0);
  cubeTex.applyTexture();
  cube.render(true);
//  cube.renderList();
//  Texture::cancelTextures();
  
  glPopMatrix();
}

// Random-related
float comRandomFloat()
{
  return (float)rand() / RAND_MAX;
}

float comRandomFloat(float min, float max)
{
  return (max - min)*comRandomFloat() + min;
}

Colour comRandomColour()
{
#ifdef DEBUG
  printf("Creating random colour.\n");
#endif
  
  float r = comRandomFloat(0.125f, 0.875f),
        g = comRandomFloat(0.125f, 0.875f),
        b = comRandomFloat(0.125f, 0.875f);
  Colour result(r, g, b);
  
  float brightness = comRandomFloat(0.5f, 0.75f);
  brightness /= result.brightness();
  result = result * brightness;
  
  return result;
}

int comRandomInt(int count)
{
  return (int)((count-1)*comRandomFloat() + 0.5f);
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

vector<string> comSplitString(string str, const string& delim)
{
  vector<string> results;
  
  unsigned int cut;
  while ((cut = str.find_first_of(delim)) != string::npos)
  {
//    if (cut > 0)
      results.push_back(str.substr(0, cut));
    str = str.substr(cut+1);
  }
  if (str.length() > 0)
    results.push_back(str);
  
  return results;
}

// Drawing-related
void comDrawRect(int x, int y, int width, int height)
{
  glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x, y+height);
    glVertex2i(x+width, y+height);
    glVertex2i(x+width, y);
  glEnd();
}

void comFillRect(int x, int y, int width, int height)
{
  glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x, y+height);
    glVertex2i(x+width, y+height);
    glVertex2i(x+width, y);
  glEnd();
}

void comLoader()
{
  Mesh::loadWavefrontObjectFile(&cube, "objects/block8.obj");
  cube.compileList(true);
  {
    SDL_Surface* surface = IMG_Load("textures/block-normals.png");
//    SDL_Surface* surface = IMG_Load("textures/checkerboard.png");
    if (surface == NULL)
      printf("Runtime warning: Failed to load cube texture from 'textures/block-normals.png'.\n");
    else
    {
      cubeTex = Texture(surface);
      printf("Common texture number: %d\n", cubeTex.getTextureIndex());
      if (cubeTex.isValid())
        printf("Common texture is valid.\n");
      else
        printf("Common texture is invalid!\n");
      SDL_FreeSurface(surface);
    }
  }
}

void comInit()
{
  static Loadable cload("Common files", comLoader);
  loaderAddLoader(&cload);
}


