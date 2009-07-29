#ifndef COMMON_HEADER
#define COMMON_HEADER

#define NO_SDL_GLEXT

// System library includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <cstdlib>
#include <string>
#include <sched.h>
// STL includes
#include <vector>
#include <list>
#include <sstream>
#include <map>
#include <stack>
// SDL includes
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "GL/glext.h"

using namespace std;

/* **************** *
 * Global constants *
 * **************** */
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define TIMER_TICK 50
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define BLOCK_MOTION_SPEED 1.0f

/* ************************** *
 * Globally-defined variables *
 * ************************** */
#ifdef MAIN_MODULE
#define MOD
#else
#define MOD extern
#endif
volatile MOD int game_score;
volatile MOD int win_width, win_height;
volatile MOD bool keys_pressed[323];
volatile MOD int num_keys_pressed;

/* ************************ *
 * Globally-defined classes *
 * ************************ */
class Colour {
  public:
    float r, g, b, a;
    Colour();
    Colour(float shade);
    Colour(float shade, float alpha);
    Colour(float red, float green, float blue);
    Colour(float red, float green, float blue, float alpha);
    
    void fillArray(float a[]) const;
    
    void apply() const;
    void apply(float alpha) const;
    void applyMaterial() const;
    void applyMaterial(int attribute) const;
    void applyMaterialShiny(float shininess) const;
    void applyMaterialAlpha(float alpha) const;
    void applyLightAttribute(int number, int attribute) const;
    void applyLightAttribute(int number, int attribute, float power) const;
    
    float brightness() const;
    Colour brighter(float v) const;
    Colour darker(float v) const;
    string toString() const;
};

class Position {
  public:
    float x, y, z;
    
    Position();
    Position(const Position& p);
    Position(float x, float y, float z);
    
    void applyTranslation() const;
    void applyTranslation(float amount) const;
    void applyNormal() const;
    void applyVertex() const;
    void applyTexCoords() const;
    
    Position operator+=(const Position& p);
    Position operator-=(const Position& p);
};

class Light {
  private:
    Position pos;
    float w;
    Colour c;
  
  public:
    Light(const Position& p, float w, const Colour& c);
    
    void apply(int number) const;
    void disable(int number) const;
};

/* *************************** *
 * Interpolation-related class *
 * *************************** */

class Bezier {
  private:
    float a, b, c, d;
    int type;
  
  public:
    Bezier(); // Creates a default equation that stays at zero
    Bezier(const Bezier& curve); // Copy constructor
    Bezier(float startV, float endV,  // The starting and ending values (f(0) = startV, f(1) = endV)
           float dStart, float dEnd); // The derivatives at the start and end (df(0) = dStart, df(1) = dEnd)
    
    float f(float t) const; // Evaluate the function at a point
    float df(float t) const; // Find the derivative at a point
    bool isFlat() const; // Returns true if this is a "flat" curve (one that always remains at zero)
    float getA() const;
    float getB() const;
    float getC() const;
    float getD() const;
    
    void changeCurve(float startV, float endV, float dStart, float dEnd);
    Bezier& operator=(const Bezier& curve);
};

/* **************************** *
 * Two-dimensional Bezier curve *
 * **************************** */
class Bezier2D {
  private:
    Bezier x, y;
  
  public:
    Bezier2D(); // Creates a default set of equations that stay at (0,0)
    Bezier2D(const Bezier2D& curve);
    Bezier2D(Bezier x, Bezier y);
    Bezier2D(float startX, float startY, float endX, float endY,
             float dStartX, float dStartY, float dEndX, float dEndY);
    
    float fx(float t) const;
    float dfx(float t) const;
    float fy(float t) const;
    float dfy(float t) const;
    bool isFlat() const;
};

/* ****************************** *
 * Three-dimensional Bezier curve *
 * ****************************** */
class Bezier3D {
  private:
    Bezier x, y, z;
  
  public:
    Bezier3D(); // Creates a default set of equations that stay at (0,0,0)
    Bezier3D(const Bezier3D& curve);
    Bezier3D(Bezier x, Bezier y, Bezier z);
    Bezier3D(Position start, Position end, Position speedStart, Position speedEnd);
    
    Position f(float t) const;
    Position df(float t) const;
    bool isFlat() const;
};

/* ************ *
 * Camera class *
 * ************ */
class Camera {
  private:
    Position pos, rotation;
    Bezier3D posOffset, rotOffset;
    float posTime, rotTime;
  
  public:
    Camera();
    Camera(Position pos);
    Camera(Position pos, Position rot);
    Camera(const Camera& c);
    
    void apply() const;
    void move(float dx, float dy, float dz);
    void move(const Position dp);
    void rotate(float rx, float ry, float rz);
    void rotate(const Position dr);
    bool animate(float dTime);
};

/* ********************** *
 * Loading-system related *
 * ********************** */
class Loadable {
  private:
    string name;
    void (*loadingFunction)();
    bool done;
    
    virtual void load();
    
  public:
    Loadable(string name);
    Loadable(string name, void (*loadingFunction)());
    unsigned int getNumItems() const;
    
    string getName() const;
    bool isDone() const;
    
    void loadItems();
};

// Add a loader to the list
void loaderAddLoader(Loadable* loader);

/*
  Start loading stuff
  Note: This must be called multiple times until loaderDoneLoading() returns true
*/
void loaderRunLoader();

// Find out how done we are
float loaderGetProgress();

// If we're done loading everything
bool loaderDoneLoading();

// Find out how many things we're loading
int loaderLoaderCount();

/* ******************************* *
 * Some generally useful functions *
 * ******************************* */
// Colour-related
Colour operator*(float s, const Colour& c);
Colour operator*(const Colour& c, float s);

// Position-related
Position operator+(const Position& a, const Position& b);
Position operator-(const Position& a, const Position& b);
Position operator*(const Position& p, float s);
Position operator*(float s, const Position& p);
Position operator/(const Position& p, float s);

// Drawing-related
void comDrawCube(float x, float y, float size, float rotation);
void comDrawSphere(float x, float y, float size, float rotation);
void comDrawBlock(float x, float y, float size, float rotation);
void comDrawTexturedCube(float x, float y, float size, float rotation);

// Random-related
float comRandomFloat();
float comRandomFloat(float min, float max);
Colour comRandomColour();
int comRandomInt(int count);

// File-related
string comReadLine(FILE* in);

// String-related
bool comStartsWith(string& a, string& b);
vector<string> comSplitSpaces(string a);
vector<string> comSplitString(string s, const string& delim);

void toggleFullscreen();
void triggerRepaint();

// Drawing-related
void comDrawRect(int x, int y, int width, int height);
void comFillRect(int x, int y, int width, int height);

void comInit();

// Math-related
/*template<class T> inline T max(T a, T b)
{
  if (a > b)
    return a;
  else
    return b;
}

template<class T> inline T min(T a, T b)
{
  if (b > a)
    return a;
  else
    return b;
}*/

template<class T> inline T abs(T a)
{
  if (a > 0)
    return a;
  else
    return -a;
}

template<class T> inline T maxMag(T a, T b)
{
  if (abs(a) > abs(b))
    return a;
  else
    return b;
}

template<class T> inline T minMag(T a, T b)
{
  if (abs(b) > abs(a))
    return a;
  else
    return b;
}


/*
  Screen ID codes:
*/
#define SPLASH_SCREEN 101
#define MAIN_MENU_SCREEN 102
#define OPTIONS_SCREEN 103
#define HELP_SCREEN 104
#define PLAY_SCREEN 105

/* ********************* *
 * User-defined includes *
 * ********************* */
// Universal headers
#include "mesh.h"
#include "fonts.h"
#include "widgets.h"
#include "texture.h"
#include "screen.h"
// Order-dependent headers
#include "shapes.h"
#include "grid.h"
// Screen headers
#include "splash.h"
#include "mainMenu.h"
#include "play.h"

#ifdef MAIN_MODULE
#define GLOBAL
#else
#define GLOBAL extern
#endif

/*
  Global variables
*/

GLOBAL Font *fallbackFont;
GLOBAL Font *largeFont;

#endif

