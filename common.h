#ifndef COMMON_HEADER
#define COMMON_HEADER

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
// STL includes
#include <vector>
#include <sstream>
#include <map>
#include <stack>
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
    void applyTexCoords();
    
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
    
    void apply(int number);
    void disable(int number);
};

/* ********************************** *
 * Screen-related classes and methods *
 * ********************************** */
class Screen {
  private:
    // Private variables
    bool needRepaint;
    bool visible;
    int id;
  
  public:
    Screen(int id);
    ~Screen();
    
    // Useful methods
    void end(); // Ends this screen and returns to the previous screen on the stack
    void replaceWith(const int screenID); // Replace this screen with another one
    void call(const int screenID); // Call up another screen on top of this one
    void markRepaint(); // Indicate that we need to repaint next time around
    void clearRepaint();
    bool needsRepaint(); // Returns true if we need a redraw
    void setVisible(bool visible);
    bool isVisible();
    bool operator==(const Screen& s);
    
    // Virtual methods:
    virtual void timerTick(); // Called for each tick of the game system timer
    virtual void prepareForShow(); // Called every time this screen becomes visible
    virtual void prepareForHide(); // Called every time this screen becomes hidden
    virtual void screenPaint(); // Called to redraw this screen. This MUST leave the OpenGL matrices as it found them!
    virtual void keyboard(const SDLKey &key); // Called whenever a key is pressed or released
    virtual void mouseButton(const SDL_MouseButtonEvent &mouse);
    virtual void mouseMotion(const SDL_MouseMotionEvent &mouse);
    virtual bool isOpaque();
};

/*
  Add a new screen with the specified ID
    Returns false if it didn't work
*/
bool screenAddNew(Screen &next, int screenID);

/*
  Activate a screen with the given ID
    Returns false if there's no screen by that code
*/
bool screenActivate(int screenID);

/*
  Pop the current screen off the "screen stack"
    If there are no screens on the stack, nothing happens
*/
void screenPop();

// Send a timer tick to the current screen
void screenTimerTick();

// Send a repaint command to the screen stack
void screenPaint();

// Send a keyboard event to the current screen
void screenKeyboard(const SDLKey &key);

// Send a mouse button event to the current screen
void screenMouseButton(const SDL_MouseButtonEvent &mouse);

// Send a mouse motion event to the current screen
void screenMouseMotion(const SDL_MouseMotionEvent &mouse);

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
#include "fonts.h"

#endif

