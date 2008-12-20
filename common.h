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
// OpenGL includes
//#include <GL/glut.h>
// STL includes
#include <vector>
#include <sstream>
// SDL includes
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_mixer.h"

using namespace std;

/* **************** *
 * Global constants *
 * **************** */
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define TIMER_TICK 100
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

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
    
    void fillArray(float a[]);
    
    void apply();
    void applyMaterial();
    void applyMaterial(int attribute);
    
    float brightness();
};

class Position {
  public:
    float x, y, z;
    
    Position();
    Position(const Position& p);
    Position(float x, float y, float z);
    
    void applyTranslation();
    void applyTranslation(float amount);
    void applyNormal();
    void applyVertex();
    
    Position operator+=(const Position& p);
};

class Light {
  private:
    Position pos;
    float w;
    Colour c;
  
  public:
    Light(const Position& p, float w, const Colour& c);
    
    void apply(int number);
    void disable(int number);
};

/* ******************************* *
 * Some generally useful functions *
 * ******************************* */
// Colour-related
Colour operator*(float s, const Colour& c);
Colour operator*(const Colour& c);
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
// Random-related
float comRandomFloat();
float comRandomFloat(float min, float max);
Colour comRandomColour();
// File-related
string comReadLine(FILE* in);
// String-related
bool comStartsWith(string& a, string& b);
vector<string> comSplitSpaces(string a);
vector<string> comSplitString(string& s, const string& delim);

/* ********************* *
 * User-defined includes *
 * ********************* */
#include "grid.h"
#include "mesh.h"
#include "shapes.h"

