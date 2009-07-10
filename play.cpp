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
                                       lamp(Position(5, -5, 5), 1, Colour(0.5f)),
                                       camera(Position(0, 15, 0), Position(0, 0, 0))
{
  // Need to load a bunch of stuff here
  
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
  gluPerspective(50, (float)getWidth()/getHeight(), 0.01f, 500.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
/*  gluLookAt(0, 0, 0,
            0, 1, 0,
            0, 0, 1);*/
  camera.apply();
  
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

void PlayScreen::timerTick(float dTime)
{
  if (controls.holdingLeft() && shape.move(-1, 0))  markRepaint();
  if (controls.holdingRight() && shape.move(1, 0))  markRepaint();
  if (controls.holdingUp() && shape.move(0, 1))  markRepaint();
  if (controls.holdingDown() && shape.move(0, -1)) markRepaint();
  
  if (shape.animate(dTime, 0.0f))
    markRepaint();
}

void PlayScreen::keyboard(const SDL_KeyboardEvent& key)
{
  controls.key(key);
/*  if (key.type == SDL_KEYUP && key.keysym.sym == SDLK_ESCAPE)
    end();*/
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

/*
  Controls class
*/
Controls::Controls()
{
  moveLeft = holdLeft = false;
  moveRight = holdRight = false;
  moveUp = holdUp = false;
  moveDown = holdDown = false;
  moveDrop = holdDrop = false;
  moveSpinLeft = holdSpinLeft = false;
  moveSpinRight = holdSpinRight = false;
}

void Controls::key(const SDL_KeyboardEvent& key)
{
  if (key.type == SDL_KEYDOWN)
  {
    switch (key.keysym.sym)
    {
      case SDLK_w:
        // Hold the block from falling (W)
        moveUp = holdUp = true;
        break;
      
      case SDLK_s:
      case SDLK_DOWN:
        // Move the block down faster (Down & S)
        moveDown = holdDown = true;
        break;
      
      case SDLK_a:
      case SDLK_LEFT:
        // Move block to the left (Left & A)
        moveLeft = holdLeft = true;
        break;
      
      case SDLK_d:
      case SDLK_RIGHT:
        // Move block to the right (Right & D)
        moveRight = holdRight = true;
        break;
      
      case SDLK_SPACE:
        // Drop block to the bottom (Spacebar)
        moveDrop = holdDrop = true;
        break;
      
      case SDLK_q:
        // Spin block to the left (Q)
        moveSpinLeft = holdSpinLeft = true;
        break;
      
      case SDLK_e:
      case SDLK_UP:
        // Spin block to the right (E)
        moveSpinRight = holdSpinRight = true;
        break;
      
      default:
        break;
    }
  }
  else if (key.type == SDL_KEYUP)
  {
    switch (key.keysym.sym)
    {
      case SDLK_w:
        // Hold the block from falling (W)
        holdUp = false;
        break;
      
      case SDLK_s:
      case SDLK_DOWN:
        // Move the block down faster (Down & S)
        holdDown = false;
        break;
      
      case SDLK_a:
      case SDLK_LEFT:
        // Move block to the left (Left & A)
        holdLeft = false;
        break;
      
      case SDLK_d:
      case SDLK_RIGHT:
        // Move block to the right (Right & D)
        holdRight = false;
        break;
      
      case SDLK_SPACE:
        // Drop block to the bottom (Spacebar)
        holdDrop = false;
        break;
      
      case SDLK_q:
        // Spin block to the left (Q)
        holdSpinLeft = false;
        break;
      
      case SDLK_e:
      case SDLK_UP:
        // Spin block to the right (E)
        holdSpinRight = false;
        break;
      
      default:
        break;
    }
  }
} // end key()

bool Controls::holdingLeft()
{
  bool result = moveLeft || holdLeft;
  moveLeft = false;
  return result;
}

bool Controls::holdingRight()
{
  bool result = moveRight || holdRight;
  moveRight = false;
  return result;
}

bool Controls::holdingUp()
{
  bool result = moveUp || holdUp;
  moveUp = false;
  return result;
}

bool Controls::holdingDown()
{
  bool result = moveDown || holdDown;
  moveDown = false;
  return result;
}

bool Controls::holdingDrop()
{
  bool result = moveDrop || holdDrop;
  moveDrop = false;
  return result;
}

bool Controls::holdingSpinLeft()
{
  bool result = moveSpinLeft || holdSpinLeft;
  moveSpinLeft = false;
  return result;
}

bool Controls::holdingSpinRight()
{
  bool result = moveSpinRight || holdSpinRight;
  moveSpinRight = false;
  return result;
}

