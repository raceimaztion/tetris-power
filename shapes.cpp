#define SHAPE_MODULE
#include "common.h"

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
  type = SHAPE_TYPE_NORMAL;
  
}


Shape::Shape(FILE *in)
{
  
}
