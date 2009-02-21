#define INTRO_MODULE
#include "common.h"

/* ********************************* *
 * The intro or splash screen module *
 * ********************************* */

IntroScreen::IntroScreen()
{
  // Nothing to do here
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
  // Nothing to do here
}

bool IntroScreen::isOpaque()
{
  return true;
}

