#include "common.h"

// Element sizes and positions
#define MENU_BUTTON_WIDTH 60
#define MENU_BUTTON_HEIGHT 25

// Button tags
#define MENU_BUTTON_TAG 0x31

const Colour BLOCK_COLOUR(0.5f, 0.4f, 0.3f);

// Main PlayScreen:
PlayScreen::PlayScreen(int screenID) : Screen(screenID),
                                       panel(0, 0, getWidth(), getHeight(), Colour(0)),
                                       menu(getWidth()-MENU_BUTTON_WIDTH-2, getHeight()-MENU_BUTTON_HEIGHT-2,
                                            MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT,
                                            Colour(0.2f, 0.2f, 0.5f), "Menu", fallbackFont, MENU_BUTTON_TAG),
                                       lamp(Position(5, -5, 5), 1, Colour(0.5f))
{
  // Need to load a bunch of stuff here
  angle = 0.0f;
  
  // Add all the widgets to the panel
  panel.addChild(&menu);
  
  // Register all callbacks
  menu.addCallback(this);
  
  // TEMP:
  {
    FILE* in = fopen("block-shapes", "r");
    if (in == NULL)
      return;
    
    shape = Shape(in);
    fclose(in);
  }
}

PlayScreen::~PlayScreen()
{
  // De-allocate anything we allocated
}

void PlayScreen::prepareForShow()
{
  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glShadeModel(GL_SMOOTH);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}
void PlayScreen::prepareForHide()
{
}

void PlayScreen::screenPaint() const
{
#ifdef DEBUG
  printf("PlayScreen::paint(): Painting playing-area.\n");
#endif
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Draw background
  glDisable(GL_DEPTH_TEST);
  glColor3f(0.1f, 0.1f, 0.2f);
  comFillRect(0, 0, getWidth(), getHeight());
  glEnable(GL_DEPTH_TEST);
  
  // TEMP: Draw a block in the middle of the screen:
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(50, (float)getWidth()/getHeight(), 0.1f, 500.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  gluLookAt(0, -5, 3,
            0, 0, 0,
            0, 0, 1);
  
//  glEnable(GL_COLOR_MATERIAL);
  
  lamp.apply(GL_LIGHT0);
//  BLOCK_COLOUR.applyMaterial();
  shape.draw();
  
//  comDrawCube(0, 0, 1.0f, angle);
  
  glDisable(GL_LIGHTING);
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  // Draw grid
  // Draw block
//  shape.draw();
  
  // Draw widgets
  panel.paint();
}

void PlayScreen::timerTick()
{
  angle += 1.0f;
  if (angle >= 360.0f)
    angle -= 360.0f;
  
  triggerRepaint();
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

