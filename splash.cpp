#define INTRO_MODULE
#include "common.h"

/* ************************ *
 * The splash screen module *
 * ************************ */

SplashScreen::SplashScreen(int screenID) : Screen(screenID)
{
  // Nothing much to do here
  
  // TEMP:
  progress = 0;
  done = false;
}

SplashScreen::~SplashScreen()
{
  // Nothing to do here
}

void SplashScreen::timerTick()
{
  // Nothing to do here
  if (done)  return;
  
  float p = progress + 0.05f;
  if (p >= 1)
  {
    progress = 1;
    done = true;
  }
  else
    progress = p;
  
  markRepaint();
}

void SplashScreen::prepareForShow()
{
  // Nothing to do here
}

void SplashScreen::prepareForHide()
{
  // Nothing to do here
}

void SplashScreen::screenPaint()
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
  {
    if (done)
      glColor3f(0, 0.5, 0);
    else 
      glColor3f(0, 0, 0);
    
    glBegin(GL_LINE_LOOP);
      glVertex2i((int)(0.75f*getWidth() + 2), getHeight() - 31);
      glVertex2i((int)(0.25f*getWidth() - 1), getHeight() - 31);
      glVertex2i((int)(0.25f*getWidth() - 1), getHeight() - 13);
      glVertex2i((int)(0.75f*getWidth() + 2), getHeight() - 13);
    glEnd();
    
    glBegin(GL_QUADS);
      glVertex2f(0.25*getWidth(), getHeight() - 30);
      glVertex2f(0.25*getWidth(), getHeight() - 15);
      glVertex2f(0.25*getWidth() + 0.5*getWidth()*progress, getHeight() - 15);
      glVertex2f(0.25*getWidth() + 0.5*getWidth()*progress, getHeight() - 30);
    glEnd();
  }
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
  
  if (!replaceWith(MAIN_MENU_SCREEN))
    end();
}

void SplashScreen::mouseMotion(const SDL_MouseMotionEvent &mouse)
{
  // Nothing to do here
}

bool SplashScreen::isOpaque()
{
  return true;
}

