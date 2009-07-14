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
                                       camera(Position(0, 25, 0), Position(0, 0, 0)),
                                       controls(0.5f),
                                       grid(GRID_WIDTH, GRID_HEIGHT)
{
  // Need to load a bunch of stuff here
  state = PLAYING;
  
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
  shape.setGrid(&grid);
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
  
  // Draw sky
  glDisable(GL_DEPTH_TEST);
  glColor3f(0.1f, 0.1f, 0.2f);
  comFillRect(0, 0, getWidth(), getHeight());
  glEnable(GL_DEPTH_TEST);
  
  // Set up the viewport for 3D
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(50, (float)getWidth()/getHeight(), 0.01f, 500.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  camera.apply();
  
  lamp.apply(GL_LIGHT0);
  
  // Move to the right place
  glTranslatef(-0.5f*grid.getWidth(), 0.0f, -0.5f*grid.getHeight());
  
  // Draw grid
  grid.render();
  // Draw block
  if (state == PLAYING)
    shape.draw();
  // Draw background object(s)
  
  // Return viewport to 2D mode
  glDisable(GL_LIGHTING);
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  // Draw widgets
  panel.paint();
}

void PlayScreen::timerTick(float dTime)
{
  if (state == PLAYING)
  {
    // Player controls
    if (controls.holdingLeft() && shape.move(-1, 0))  markRepaint();
    if (controls.holdingRight() && shape.move(1, 0))  markRepaint();
    if (controls.holdingUp() && shape.move(0, 1))  markRepaint();
    if (controls.holdingDown() && shape.move(0, -1))  markRepaint();
    if (controls.holdingSpinLeft() && shape.rotateLeft())  markRepaint();
    if (controls.holdingSpinRight() && shape.rotateRight())  markRepaint();
    if (controls.holdingDrop())
    {
      shape.putInGrid();
      state = DROPPING_BLOCK;
      shape.prep();
      markRepaint();
    }
    else
    {
      if (shape.animate(dTime, 0.0f))
        markRepaint();
      if (grid.timerTick(dTime))
        markRepaint();
    }
  }
  else if (state == DROPPING_BLOCK)
  {
    if (!grid.timerTick(dTime))
    {
      if (controls.holdingDrop())
        state = WAITING;
      else
        state = PLAYING;
    }
  }
  else if (state == WAITING)
  {
    if (!controls.holdingDrop())
      state = PLAYING;
  }
  
  controls.timerTick(dTime);
}

void PlayScreen::keyboard(const SDL_KeyboardEvent& key)
{
  controls.keyEvent(key);
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
  Single Control class
*/
Control::Control()
{
  keyHeld = keyPressed = false;
  repeatTime = timeLeft = 0.0f;
}

Control::Control(float repeatTime)
{
  keyHeld = keyPressed = false;
  this->repeatTime = repeatTime;
  timeLeft = 0.0f;
}

void Control::keyEvent(bool pressed)
{
  keyHeld = pressed;
  keyPressed |= pressed;
  if (pressed)
    timeLeft = 0.0f;
}

bool Control::isPressed()
{
  if (timeLeft > 0.0f)
    return false;
  
  bool result = keyHeld | keyPressed;
  timeLeft = repeatTime;
  keyPressed = false;
  return result;
}

void Control::timerTick(float dTime)
{
  timeLeft -= dTime;
  if (timeLeft < 0.0f)  timeLeft = 0.0f;
}

float Control::getRepeatTime() const
{
  return repeatTime;
}

void Control::setRepeatTime(float repeatTime)
{
  this->repeatTime = repeatTime;
}

/*
  Controls class
*/
Controls::Controls()
{
  // Nothing much to do here
  numPressed = 0;
}

Controls::Controls(float repeatTime) : left(repeatTime), right(repeatTime),
                                       up(repeatTime), down(repeatTime),
                                       drop(repeatTime), spinLeft(repeatTime),
                                       spinRight(repeatTime)
{
  // Nothing much to do here
  numPressed = 0;
}

void Controls::keyEvent(const SDL_KeyboardEvent& key)
{
  if (key.type != SDL_KEYUP && key.type != SDL_KEYDOWN)
    return;
  
  bool pressed = (key.type == SDL_KEYDOWN);
  int dir = (pressed ? 1 : -1);
  numPressed += dir;
  
  switch (key.keysym.sym)
  {
    case SDLK_w:
      // Hold the block from falling (W)
      up.keyEvent(pressed);
      break;
      
    case SDLK_s:
    case SDLK_DOWN:
      // Move the block down faster (Down & S)
      down.keyEvent(pressed);
      break;
    
    case SDLK_a:
    case SDLK_LEFT:
      // Move block to the left (Left & A)
      left.keyEvent(pressed);
      break;
    
    case SDLK_d:
    case SDLK_RIGHT:
      // Move block to the right (Right & D)
      right.keyEvent(pressed);
      break;
    
    case SDLK_SPACE:
      // Drop block to the bottom (Spacebar)
      drop.keyEvent(pressed);
      break;
    
    case SDLK_q:
      // Spin block to the left (Q)
      spinLeft.keyEvent(pressed);
      break;
    
    case SDLK_e:
    case SDLK_UP:
      // Spin block to the right (E)
      spinRight.keyEvent(pressed);
      break;
    
    default:
      numPressed -= dir;
      break;
  }
  
  if (numPressed < 0)
    numPressed = 0;
} // end keyEvent()

bool Controls::holdingLeft()
{
  return left.isPressed();
}

bool Controls::holdingRight()
{
  return right.isPressed();
}

bool Controls::holdingUp()
{
  return up.isPressed();
}

bool Controls::holdingDown()
{
  return down.isPressed();
}

bool Controls::holdingDrop()
{
  return drop.isPressed();
}

bool Controls::holdingSpinLeft()
{
  return spinLeft.isPressed();
}

bool Controls::holdingSpinRight()
{
  return spinRight.isPressed();
}

bool Controls::anyKeyPressed()
{
  return numPressed != 0;
}

void Controls::timerTick(float dTime)
{
  left.timerTick(dTime);
  right.timerTick(dTime);
  up.timerTick(dTime);
  down.timerTick(dTime);
  drop.timerTick(dTime);
  spinLeft.timerTick(dTime);
  spinRight.timerTick(dTime);
}

float Controls::getRepeatTime() const
{
  return up.getRepeatTime();
}

void Controls::setRepeatTime(float repeatTime)
{
  left.setRepeatTime(repeatTime);
  right.setRepeatTime(repeatTime);
  up.setRepeatTime(repeatTime);
  down.setRepeatTime(repeatTime);
  drop.setRepeatTime(repeatTime);
  spinLeft.setRepeatTime(repeatTime);
  spinRight.setRepeatTime(repeatTime);
}

