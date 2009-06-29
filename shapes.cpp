#define SHAPE_MODULE
#include "common.h"

#define SMALL 0.0000001f

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
  type = SHAPE_TYPE_NOT_READY;
  offset_x = offset_y = 0.0f;
  distGone_x = distGone_y = 0.0f;
}


Shape::Shape(FILE *in)
{
  type = SHAPE_TYPE_NOT_READY;
  
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
  int count, x, y;
  fscanf(in, "%d %d\n", &size, &count);
  
  for (int i=0; i < count; i++)
  {
    fscanf(in, "%d %d\n", &x, &y);
    the_bits.push_back(ABit(x, y));
  }
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
  
  offset_x -= dx;
  offset_y -= dy;
  
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
  if (abs(offset_x) < SMALL)
    distGone_x = 0.0f;
  else
  {
    float delta = minMag(distGone_x + 0.001f, offset_x);
    offset_x -= delta;
    distGone_x -= delta;
    result = true;
  }
  
  return false;
}

