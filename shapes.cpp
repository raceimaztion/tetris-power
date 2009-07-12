#define SHAPE_MODULE
#include "common.h"

#define SMALL 0.001f
#define SHAPE_SPEED 2.0f
#define SHAPE_MOVE_TIME 0.5f
#define CURVE_END_SLOPE 0.0f

// Variables private to the Shapes module:
Mesh block;


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
  
//  offset_x = offset_y = 0.0f;
//  start_x = start_y = 0.0f;
}

void Shape::addBit(ABit bit)
{
  the_bits.push_back(bit);
}

void Shape::prep()
{
  pos.x = (GRID_WIDTH - size) / 2;
  pos.y = GRID_HEIGHT;
}

bool Shape::collides() const
{
  // TODO: Add stuff to the grid module
  return false;
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
  return false;
}

bool Shape::rotateLeft()
{
  return false;
}

bool Shape::animate(float dTime, float curTime)
{
  bool result = false;
  
  if (!offsetX.isFlat())
  {
    timeX += dTime/SHAPE_MOVE_TIME;
    if (timeX > 1.0f)
    {
#ifdef DEBUG
      printf("End X curve.\n");
#endif
      offsetX = Bezier();
    }
#ifdef DEBUG
    else
      printf("Continuing X curve.\n");
#endif
    
    result = true;
  }
  
  if (!offsetY.isFlat())
  {
    timeY += dTime/SHAPE_MOVE_TIME;
    if (timeY > 1.0f)
    {
#ifdef DEBUG
      printf("End Y curve.\n");
#endif
      offsetY = Bezier();
    }
#ifdef DEBUG
    else
      printf("Continuing Y curve.\n");
#endif
    
    result = true;
  }
  
  return result;
}

void Shape::draw() const
{
  glPushMatrix();
  
  glTranslatef(pos.x, 0, pos.y);
//  glTranslatef(offset_x, 0, offset_y);
  glTranslatef(offsetX.f(timeX), 0, offsetY.f(timeY));
  c.applyMaterial();
  for (int i=the_bits.size()-1; i >= 0; i--) 
  {
    comDrawCube(the_bits.at(i).pos.x, the_bits.at(i).pos.y, 0.5f, 0);
  }
  
  glPopMatrix();
}

// Init function for the Shapes module
void shInit()
{
  
}

