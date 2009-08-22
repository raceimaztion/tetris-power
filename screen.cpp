#define SCREEN_MODULE
#include "common.h"

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
  // Nothing to do here
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
    glFlush();
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


