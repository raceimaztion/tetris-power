#include "common.h"

// Button tags:
#define MENU_TAG_START 0x21
#define MENU_TAG_OPTIONS 0x22
#define MENU_TAG_QUIT 0x23

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 30
#define BUTTON_SPACING 40
#define BUTTON_TOP (getHeight() - 2*BUTTON_SPACING - 3*BUTTON_HEIGHT)/2

const Colour MENU_BUTTON_COLOUR(0.5f, 0.65f, 0.5f),
             TITLE_LABEL_COLOUR(0.6f);

MainMenu::MainMenu(int screenID) : Screen(screenID),
                                   panel(0, 0, getWidth(), getHeight(), Colour(0)),
                                   title(5, 40, getWidth()-10, 20, TITLE_LABEL_COLOUR, "Tetris-Power", largeFont),
                                   start((getWidth()-BUTTON_WIDTH)/2, BUTTON_TOP,
                                         BUTTON_WIDTH, BUTTON_HEIGHT,
                                         MENU_BUTTON_COLOUR, "Start", largeFont, MENU_TAG_START),
                                   options((getWidth()-BUTTON_WIDTH)/2, BUTTON_TOP + BUTTON_SPACING + BUTTON_HEIGHT,
                                           BUTTON_WIDTH, BUTTON_HEIGHT,
                                           MENU_BUTTON_COLOUR, "Options", largeFont, MENU_TAG_OPTIONS),
                                   quit((getWidth()-BUTTON_WIDTH)/2, BUTTON_TOP + 2*(BUTTON_SPACING + BUTTON_HEIGHT),
                                        BUTTON_WIDTH, BUTTON_HEIGHT,
                                        MENU_BUTTON_COLOUR, "Quit", largeFont, MENU_TAG_QUIT)
{
  // Nothing much to do here
  panel.addChild(&start);
  panel.addChild(&options);
  panel.addChild(&quit);
  
  panel.addChild(&title);
  
  start.addListener(this);
  options.addListener(this);
  quit.addListener(this);
}

void MainMenu::timerTick(float dTime)
{
  panel.timerTick(dTime);
  if (quit.getFont() == NULL && fallbackFont != NULL)
    panel.setFont(fallbackFont);
  if (panel.needsRepaint())
  {
    markRepaint();
    panel.clearRepaint();
  }
}
void MainMenu::prepareForShow()
{
#ifdef DEBUG
  printf("Starting to display the main menu.\n");
#endif
}

void MainMenu::prepareForHide()
{
#ifdef DEBUG
  printf("Ending the main menu.\n");
#endif
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

bool MainMenu::isOpaque() const
{
  return true;
}

void MainMenu::buttonCallback(const Button& b)
{
  switch (b.getTag())
  {
    case MENU_TAG_START:
      // Start the game here
      // TODO: Ask what difficulty level and game type to use
      call(PLAY_SCREEN);
      break;
    
    case MENU_TAG_OPTIONS:
      // Show the Options Screen
      break;
    
    case MENU_TAG_QUIT:
      end();
      break;
  }
}

void MainMenu::screenPaint() const
{
  glColor3f(0.1f,0.3f,0.1f);
  comFillRect(0, 0, getWidth(), getHeight());
  
  panel.paint();
}

