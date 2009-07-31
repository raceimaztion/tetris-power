#include "common.h"

// Element sizes and positions
#define MENU_BUTTON_WIDTH 60
#define MENU_BUTTON_HEIGHT 20

// Button tags
#define MENU_BUTTON_TAG 0x31

// Timing
#define ROW_DROP_TIME 1.0f
#define ROW_FASTER_FACTOR 2.0f

const Colour BLOCK_COLOUR(0.5f, 0.4f, 0.3f),
             BACKDROP_COLOUR(0.3f, 0.3f, 0.3f);

// Main PlayScreen:
PlayScreen::PlayScreen(int screenID) : Screen(screenID),
                                       Loadable("Backdrop"),
                                       panel(0, 0, getWidth(), getHeight(), Colour(0)),
                                       floatables(0, 0, getWidth(), getHeight(), Colour(0)),
                                       menu(getWidth()-MENU_BUTTON_WIDTH-2, getHeight()-MENU_BUTTON_HEIGHT-2,
                                            MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT,
                                            Colour(0.5f, 0.5f, 0.7f), "Menu", fallbackFont, MENU_BUTTON_TAG),
                                       scoreView(50, 50, 200, 15,
                                                 Colour(0.8f, 0.7f, 0.5f), fallbackFont),
                                       lamp(Position(5, -5, 5), 1, Colour(0.5f)),
                                       camera(Position(0, 25, 0), Position(0, 0, 0)),
                                       grid(GRID_WIDTH, GRID_HEIGHT),
                                       shape(shRandomShape(&grid)),
                                       nextShape(shRandomShape(&grid)),
                                       controls(0.25f)
{
  // Need to load a bunch of stuff here
  loaderAddLoader(this);
  state = PLAYING;
  dropTime = ROW_DROP_TIME;
  
  // Add all the widgets to the panel
  panel.addChild(&menu);
  panel.addChild(&scoreView);
  
  // Register all callbacks
  menu.addCallback(this);
  grid.addListener(this);
  
  shape.setGrid(&grid);
  shape.prepForUse();
  
  scoreView.setAlignment(0.0f, 0.5f);
}

PlayScreen::~PlayScreen()
{
  // De-allocate anything we allocated
}

void PlayScreen::prepareForShow()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  
  // Sky colour
  glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
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
  
  // Set up the viewport for 3D
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(50, (float)getWidth()/getHeight(), 0.1f, 50.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  camera.apply();
  
  lamp.apply(GL_LIGHT0);
  
  // TEMP:
  /*
  glBindTexture(GL_TEXTURE_2D, (GLuint)1);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
  glEnd();
  //*/
  
  BACKDROP_COLOUR.applyMaterial();
  backdrop.render();
  
  // Move to the right place
  glTranslatef(-0.5f*grid.getWidth(), 0.0f, -0.5f*grid.getHeight());
  
  // Draw grid
  grid.render();
  // Draw block
  if (state == PLAYING)
    shape.draw();
  // Draw next shape
  glPushMatrix();
  glTranslatef(grid.getWidth() + 4 - 0.5f*nextShape.getSize(), 0,
               grid.getHeight() - 2 - 0.5f*nextShape.getSize());
  nextShape.draw();
  glPopMatrix();
  // Draw background object(s)
  
  // Return viewport to 2D mode
  glDisable(GL_LIGHTING);
  
  glDisable(GL_TEXTURE_2D);
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  // Draw widgets
  panel.paint();
  
  // Draw floatables
  floatables.paint();
}

void PlayScreen::timerTick(float dTime)
{
  if (state == PLAYING)
  {
    // Player controls
    // left and right
    if (controls.holdingLeft() && shape.move(-1, 0))  markRepaint();
    if (controls.holdingRight() && shape.move(1, 0))  markRepaint();
    // up (hold) and down (faster)
    if (!controls.holdingUp())
    {
      if (controls.holdingDown())
        dropTime -= ROW_FASTER_FACTOR * dTime;
      else
        dropTime -= dTime;
      
      if (dropTime < 0.0f)
      {
        dropTime = ROW_DROP_TIME;
        if (!shape.move(0, -1))
          putShapeInGrid();
      }
    }
    // spin left and right
    if (controls.holdingSpinLeft() && shape.rotateLeft())  markRepaint();
    if (controls.holdingSpinRight() && shape.rotateRight())  markRepaint();
    // drop shape
    if (controls.holdingDrop())
      dropShape();
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
      // The grid is finished moving blocks down, show the next shape
      nextShape = shRandomShape(&grid);
      controls.flushKeys();
      
      // If the drop key is still held, wait until it's not
      if (controls.holdingDrop())
        state = WAITING;
      else
        state = PLAYING;
    }
    markRepaint();
  }
  else if (state == WAITING)
  {
    if (!controls.holdingDrop())
    {
      state = PLAYING;
      markRepaint();
    }
  }
  
  controls.timerTick(dTime);
  panel.timerTick(dTime);
  floatables.timerTick(dTime);
  {
    list<FloatyLabel*> finished;
    for (list<FloatyLabel>::iterator cur = floatingLabels.begin(); cur != floatingLabels.end(); cur++)
    {
      if ((*cur).isDone())
        finished.push_back(&(*cur));
    }
    for (list<FloatyLabel*>::iterator cur = finished.begin(); cur != finished.end(); cur++)
    {
      floatables.removeChild(*cur);
      floatingLabels.remove(**cur);
    }
  }
} // end timerTick()

void PlayScreen::keyboard(const SDL_KeyboardEvent& key)
{
  controls.keyEvent(key);
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

void PlayScreen::dropShape()
{
  int curY = shape.getY();
  while (shape.move(0, -1)) ;
  putShapeInGrid(shape.getY() - curY);
  controls.flushKeys();
}

void PlayScreen::putShapeInGrid(int distance)
{
  shape.putInGrid(distance);
  state = DROPPING_BLOCK;
  dropTime = 2 * ROW_DROP_TIME;
  
  shape = nextShape;
  // The next shape will be changed when the block stops falling
  
  shape.setGrid(&grid);
  shape.prepForUse();
  markRepaint();
}

void PlayScreen::rowRemoved(int row)
{
  floatingLabels.push_back(FloatyLabel(0, getHeight() - (row*21 + 42), getWidth(), 1,
                                     Colour(0.8f, 0.9f, 0.7f), "+100", fallbackFont,
                                     2.0f, -20.0f));
  floatables.addChild(&floatingLabels.back());
  
  scoreAdd(100);
}

void PlayScreen::gridEmpty()
{
  floatingLabels.push_back(FloatyLabel(0, 0, getWidth(), getHeight(),
                                     Colour(0.8f, 0.9f, 0.7f), "+150", largeFont,
                                     2.0f, -20.0f));
  floatables.addChild(&floatingLabels.back());
  
  scoreAdd(150);
}

void PlayScreen::load()
{
  // Load the backdrop
  Mesh::loadWavefrontObjectFile(&backdrop, "objects/backdrop.obj");
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

void Control::flush()
{
  keyPressed = keyHeld;
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
                                       up(0.0f), down(0.0f),
                                       drop(0.0f), spinLeft(repeatTime),
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

void Controls::flushKeys()
{
  left.flush();
  right.flush();
  up.flush();
  down.flush();
  drop.flush();
  spinLeft.flush();
  spinRight.flush();
}

