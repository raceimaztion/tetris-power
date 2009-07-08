#define INTRO_MODULE
#include "common.h"

/* ************************ *
 * The splash screen module *
 * ************************ */

#define PROGRESS_HEIGHT 30

SplashScreen::SplashScreen(int screenID) : Screen(screenID),
                                           panel(0, 0, getWidth(), getHeight(), Colour(0)),
                                           progress(getWidth()/4, getHeight() - 2*PROGRESS_HEIGHT,
                                                    getWidth()/2, PROGRESS_HEIGHT,
                                                    Colour(0.2f, 0.6f, 0.2f))
{
  // Nothing much to do here
  panel.addChild(&progress);
}

SplashScreen::~SplashScreen()
{
  // Nothing to do here
}

void SplashScreen::timerTick(float dTime)
{
  // Nothing to do here
  if (!loaderDoneLoading() || (progress.getPercentage() < 1.0f))
  {
    progress.setPercentage(loaderGetProgress());
    markRepaint();
  }
  else // TEMP:
  {
    if (!replaceWith(MAIN_MENU_SCREEN))
      end();
  }
}

void SplashScreen::prepareForShow()
{
  // Nothing much to do here
  glDisable(GL_DEPTH_TEST);
}

void SplashScreen::prepareForHide()
{
  // Nothing to do here
}

void SplashScreen::screenPaint() const
{
  // Draw some fancy background
  glColor3f(0.5f, 0.4f, 0.2f);
  glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, getHeight());
    glVertex2f(getWidth(), getHeight());
    glVertex2f(getWidth(), 0);
  glEnd();
  
  panel.paint();
}

void SplashScreen::keyboard(const SDL_KeyboardEvent &key)
{
  if (key.keysym.scancode && !replaceWith(MAIN_MENU_SCREEN))
    end();
  
#ifdef DEBUG
  printf("Splash debug: Keyboard event:\n");
  if (key.type == SDL_KEYDOWN)
    printf("Event type: Key down.\n");
  else if (key.type == SDL_KEYUP)
    printf("Event type: Key up.\n");
  else
    printf("Event type: Unknown.\n");
  printf("Key scancode: %d\n", key.keysym.scancode);
#endif
}

void SplashScreen::mouseButton(const SDL_MouseButtonEvent &mouse)
{
#ifdef DEBUG
  printf("Splash debug: Mouse button event:\n");
  if (mouse.type == SDL_MOUSEBUTTONDOWN)
    printf("Event type: Mouse button down.\n");
  else if (mouse.type == SDL_MOUSEBUTTONUP)
    printf("Event type: Mouse button up.\n");
  else
    printf("Event type: Unknown.\n");
  printf("Mouse position: %d, %d\n", mouse.x, mouse.y);
#endif
  
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

