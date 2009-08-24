#define INTRO_MODULE
#include "common.h"

/* ************************ *
 * The splash screen module *
 * ************************ */

#define PROGRESS_HEIGHT 30

SplashScreen::SplashScreen(int screenID) : Screen(screenID),
                                           panel(0, 0, getWidth(), getHeight(), Colour(0)),
                                           status(0, getHeight() - 2*PROGRESS_HEIGHT - 20,
                                                  getWidth(), 15,
                                                  Colour(0.7f), "Click anywhere to continue.", fallbackFont),
                                           title(0, 20, getWidth(), 30,
                                                 Colour(0.6f), "Tetris-Power", largeFont)
{
  // Nothing much to do here
  panel.addChild(&status);
  panel.addChild(&title);
  
  timerTick(0.0f);
}

SplashScreen::~SplashScreen()
{
  // Nothing to do here
}

void SplashScreen::timerTick(float dTime)
{
  // Nothing much to do here
  panel.timerTick(dTime);
}

void SplashScreen::prepareForShow()
{
  // Nothing much to do here
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.5f, 0.4f, 0.2f, 1.0f);
}

void SplashScreen::prepareForHide()
{
  // Nothing to do here
}

void SplashScreen::screenPaint() const
{
  // TODO: Draw some fancy background
  
  panel.paint();
}

void SplashScreen::keyboard(const SDL_KeyboardEvent &key)
{
  if (key.keysym.scancode && !replaceWith(MAIN_MENU_SCREEN))
    end();
}

void SplashScreen::mouseButton(const SDL_MouseButtonEvent &mouse)
{
  if (mouse.type == SDL_MOUSEBUTTONUP && !replaceWith(MAIN_MENU_SCREEN))
    end();
}

void SplashScreen::mouseMotion(const SDL_MouseMotionEvent &mouse)
{
  // Nothing to do here
}

bool SplashScreen::isOpaque() const
{
  return true;
}

