#include "common.h"

MainMenu::MainMenu(int screenID) : Screen(screenID),
                                   panel(0, 0, getWidth(), getHeight(), Colour(0)),
                                   quit(100, 100, 50, 20, Colour(0.6f), "Quit", fallbackFont, MAINMENU_TAG_QUIT),
                                   label(100, 80, 100, 20, Colour(0.6f), "Test", fallbackFont)
{
  // Nothing much to do here
  panel.addChild(&quit);
  panel.addChild(&label);
  quit.addCallback(this);
}

void MainMenu::timerTick()
{
  panel.timerTick();
  if (quit.getFont() == NULL && fallbackFont != NULL)
  {
    quit.setFont(fallbackFont);
    label.setFont(fallbackFont);
  }
}
void MainMenu::prepareForShow()
{
  printf("Starting to display the main menu.\n");
}

void MainMenu::prepareForHide()
{
  printf("Ending the main menu.\n");
}

void MainMenu::keyboard(const SDL_KeyboardEvent& key)
{
  if (key.keysym.sym == SDLK_ESCAPE)
    end();
}

void MainMenu::mouseButton(const SDL_MouseButtonEvent& mouse)
{
  panel.mouse(mouse);
}
void MainMenu::mouseMotion(const SDL_MouseMotionEvent& mouse)
{
  panel.mouse(mouse);
}

bool MainMenu::isOpaque()
{
  return true;
}

void MainMenu::buttonCallback(const Button& b)
{
  if (b.getTag() == MAINMENU_TAG_QUIT)
    end();
}

void MainMenu::screenPaint()
{
  glColor3f(0.1f,0.5f,0.1f);
  glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, getHeight());
    glVertex2f(getWidth(), getHeight());
    glVertex2f(getWidth(), 0);
  glEnd();
  
  panel.paint();
}

