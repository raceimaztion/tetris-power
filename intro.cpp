#define INTRO_MODULE
#include "common.h"

/* ********************************* *
 * The intro or splash screen module *
 * ********************************* */

IntroScreen::IntroScreen(int screenID) : Screen(screenID)
{
  // Nothing much to do here
  
  // TEMP:
  mouse_x = mouse_y = 0;
}

IntroScreen::~IntroScreen()
{
  // Nothing to do here
}

void IntroScreen::timerTick()
{
  // Nothing to do here
}

void IntroScreen::prepareForShow()
{
  // Nothing to do here
}

void IntroScreen::prepareForHide()
{
  // Nothing to do here
}

void IntroScreen::screenPaint()
{
  // Draw some fancy background
  glColor3f(0.5f, 0.4f, 0.2f);
  glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, getHeight());
    glVertex2f(getWidth(), getHeight());
    glVertex2f(getWidth(), 0);
  glEnd();
  
  // TEMP:
  glColor3f(0, 0, 0);
  glBegin(GL_LINES);
    glVertex2f(mouse_x-5, mouse_y);
    glVertex2f(mouse_x+5, mouse_y);
    glVertex2f(mouse_x, mouse_y-5);
    glVertex2f(mouse_x, mouse_y+5);
  glEnd();
}

void IntroScreen::keyboard(const SDL_keysym &key)
{
  if (!replaceWith(MAIN_MENU_SCREEN))
    end();
}

void IntroScreen::mouseButton(const SDL_MouseButtonEvent &mouse)
{
  if (!replaceWith(MAIN_MENU_SCREEN))
    end();
}

void IntroScreen::mouseMotion(const SDL_MouseMotionEvent &mouse)
{
  // Nothing much to do here
  
  // TEMP:
  if (mouse_x != mouse.x)
  {
    mouse_x = mouse.x;
    markRepaint();
  }
  if (mouse_y != mouse.y)
  {
    mouse_y = mouse.y;
    markRepaint();
  }
}

bool IntroScreen::isOpaque()
{
  return true;
}

