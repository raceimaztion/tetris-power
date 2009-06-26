#include "common.h"

// Element sizes and positions
#define MENU_BUTTON_WIDTH 60

// Button tags
#define MENU_BUTTON_TAG 0x31

// Main PlayScreen:
PlayScreen::PlayScreen(int screenID) : Screen(screenID),
                                       panel(0, 0, getWidth(), getHeight(), Colour(0)),
                                       quit(getWidth()-MENU_BUTTON_WIDTH-2, getHeight()-22,
                                            MENU_BUTTON_WIDTH, 20,
                                            Colour(0.2f, 0.2f, 0.5f), "Menu", fallbackFont, MENU_BUTTON_TAG)
{
  // Need to load a bunch of stuff here
  
  // Add all the widgets to the panel
  panel.addChild(&quit);
  
  // Register all callbacks
  quit.addCallback(this);
}

PlayScreen::~PlayScreen()
{
  // De-allocate anything we allocated
}

void PlayScreen::prepareForShow()
{
}
void PlayScreen::prepareForHide()
{
}

void PlayScreen::screenPaint() const
{
#ifdef DEBUG
  printf("PlayScreen::paint(): Painting playing-area.\n");
#endif
  // Draw background
  glColor3f(0.1f, 0.1f, 0.2f);
  fillRect(0, 0, getWidth(), getHeight());
  
  // Draw grid
  // Draw block
  // Draw widgets
  panel.paint();
}

void PlayScreen::timerTick()
{
}

void PlayScreen::keyboard(const SDL_KeyboardEvent& key)
{
  if (key.type == SDL_KEYUP && key.keysym.sym == SDLK_ESCAPE)
    end();
}

void PlayScreen::mouseButton(const SDL_MouseButtonEvent& mouse)
{
  panel.mouse(mouse);
}

void PlayScreen::mouseMotion(const SDL_MouseMotionEvent& mouse)
{
  panel.mouse(mouse);
}

bool PlayScreen::isOpaque() const
{
  return true;
}

void PlayScreen::buttonCallback(const Button& b)
{
  switch(b.getTag())
  {
    case MENU_BUTTON_TAG:
      end();
      break;
  }
}

