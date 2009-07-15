#define SHAPE_MODULE
#include "common.h"

#define SMALL 0.001f
#define SHAPE_SPEED 2.0f
#define SHAPE_MOVE_TIME 0.5f
#define SHAPE_ROT_TIME 0.5f
#define CURVE_END_SLOPE 0.0f

/* *********** *
 * Point class *
 * *********** */
Point::Point()
{
  x = y = 0;
}

Point::Point(const Point& p)
{
  x = p.x;
  y = p.y;
}

Point::Point(int x, int y)
{
  this->x = x;
  this->y = y;
}

bool Point::operator==(const Point& p)
{
  return x == p.x && y == p.y;
}

/* *********** *
 * Shape class *
 * *********** */
Shape::Shape()
{
  init();
}

Shape::Shape(FILE *in)
{
  if (in == NULL || feof(in))
    return;
  
  /*
    Block file format:
    3 4  # Block "header", [block size], [number of bits in block]
    1 0  # One line per block
    1 1
    1 2
    2 2
  */
  init();
  int count, x, y;
  fscanf(in, "%d %d\n", &size, &count);
  
  for (int i=0; i < count; i++)
  {
    fscanf(in, "%d %d\n", &x, &y);
    addBit(ABit(x, y));
  }
}

Shape::Shape(const Shape& s)
{
  type = s.type;
  size = s.size;
  c = s.c;
  pos = s.pos;
  
}

void Shape::init()
{
  type = SHAPE_TYPE_NOT_READY;
  pos.x = pos.y = 0;
  c = comRandomColour();
  grid = NULL;
  timeX = timeY = timeRot = 0.0f;
}

void Shape::rotLeft()
{
  for (int i=the_bits.size()-1; i >= 0; i--)
  {
    int temp = the_bits.at(i).pos.y + 1;
    the_bits.at(i).pos.y = the_bits.at(i).pos.x;
    the_bits.at(i).pos.x = size - temp;
  }
}

void Shape::rotRight()
{
  for (int i=the_bits.size()-1; i >= 0; i--)
  {
    int temp = the_bits.at(i).pos.x + 1;
    the_bits.at(i).pos.x = the_bits.at(i).pos.y;
    the_bits.at(i).pos.y = size - temp;
  }
}

void Shape::addBit(ABit bit)
{
  the_bits.push_back(bit);
}

void Shape::prep()
{
  if (grid == NULL)
  {
    pos.x = (GRID_WIDTH - size) / 2;
    pos.y = GRID_HEIGHT;
  }
  else
  {
    pos.x = (grid->getWidth() - size) / 2;
    pos.y = grid->getHeight()-size;
  }
}

bool Shape::collides() const
{
  if (grid == NULL)
    return false;
  else
  {
    for (int i=the_bits.size()-1; i >= 0; i--)
      if (grid->isCellOccupied(pos.x+the_bits.at(i).pos.x, pos.y+the_bits.at(i).pos.y))
        return true;
    return false;
  }
}

bool Shape::move(int dx, int dy)
{
  pos.x += dx;
  pos.y += dy;
  
  // If we bumped into something, undo motion and return
  if (collides())
  {
    pos.x -= dx;
    pos.y -= dy;
    return false;
  }
  
#ifdef DEBUG
  printf("Shape (x,y): (%d, %d)\n", pos.x, pos.y);
#endif
  
  if (dx != 0)
  {
    Bezier tempX(offsetX.f(timeX) - dx, 0.0f, offsetX.df(timeX), CURVE_END_SLOPE);
    offsetX = tempX;
    timeX = 0.0f;
  }
  if (dy != 0)
  {
    Bezier tempY(offsetY.f(timeY) - dy, 0.0f, offsetY.df(timeY), CURVE_END_SLOPE);
    offsetY = tempY;
    timeY = 0.0f;
  }
  
  return true;
}

bool Shape::rotateRight()
{
  // TODO: Fill this in
  rotRight();
  if (collides())
  {
    rotLeft();
    return false;
  }
  
  Bezier tempRot(offsetRot.f(timeRot) - 90, 0.0f, offsetRot.df(timeRot), CURVE_END_SLOPE);
  offsetRot = tempRot;
  timeRot = 0.0f;
  
  return false;
}

bool Shape::rotateLeft()
{
  // TODO: Fill this in
  rotLeft();
  if (collides())
  {
    rotRight();
    return false;
  }
  
  Bezier tempRot(offsetRot.f(timeRot) + 90, 0.0f, offsetRot.df(timeRot), CURVE_END_SLOPE);
  offsetRot = tempRot;
  timeRot = 0.0f;
  
  return false;
}

bool Shape::animate(float dTime, float curTime)
{
  bool result = false;
  
  if (!offsetX.isFlat())
  {
    timeX += dTime/SHAPE_MOVE_TIME;
    if (timeX > 1.0f)
      offsetX = Bezier();
    
    result = true;
  }
  
  if (!offsetY.isFlat())
  {
    timeY += dTime/SHAPE_MOVE_TIME;
    if (timeY > 1.0f)
      offsetY = Bezier();
    
    result = true;
  }
  
  if (!offsetRot.isFlat())
  {
    timeRot += dTime/SHAPE_ROT_TIME;
    if (timeRot > 1.0f)
      offsetRot = Bezier();
    
    result = true;
  }
  
  return result;
}

void Shape::draw() const
{
  glPushMatrix();
  
  glTranslatef(pos.x, 0, pos.y);
  glTranslatef(offsetX.f(timeX), 0, offsetY.f(timeY));
  
  glTranslatef(0.5f*size, 0.0f, 0.5f*size);
  glRotatef(offsetRot.f(timeRot), 0, 1, 0);
  glTranslatef(-0.5f*size, 0.0f, -0.5f*size);
  
  c.applyMaterial();
  for (int i=the_bits.size()-1; i >= 0; i--) 
  {
    comDrawCube(the_bits.at(i).pos.x, the_bits.at(i).pos.y, 0.5f, 0);
  }
  
  glPopMatrix();
}

Grid* Shape::getGrid() const
{
  return grid;
}

void Shape::setGrid(Grid* grid)
{
  this->grid = grid;
}

void Shape::putInGrid()
{
  if (grid == NULL)
  {
    printf("Runtime warning: Shape::putInGrid(): Can't put the shape into the grid if the grid pointer is NULL.\n");
    return;
  }
  
  for (int i=the_bits.size()-1; i >= 0; i--)
    grid->placeBit(the_bits.at(i), c, pos.x, pos.y);
}

// Init function for the Shapes module
void shInit()
{
  srand(time(NULL));
}

