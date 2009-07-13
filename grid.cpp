#include "common.h"

#define ACCELERATION 0.5f

/* ************* *
 * GridBit class *
 * ************* */
GridBit::GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, Grid *grid)
                                 : ABit(bit.pos.x + offsetX, bit.pos.y + offsetY)
{
  this->c = c;
  this->grid = grid;
  offsetX = speedX = 0.0f;
}

bool GridBit::timerTick(float dTime)
{
  if (offsetX < 0.0f)
  {
    speedX += ACCELERATION*dTime;
    offsetX += speedX*dTime;
    if (offsetX > 0.0f)
      offsetX = speedX = 0.0f;
    
    return true;
  }
  else
    return false;
}

void GridBit::render() const
{
  c.applyMaterial();
  comDrawCube(pos.x, pos.y, 0.5f, 0);
}

bool GridBit::drop()
{
  int y = pos.y - 1;
  while (!grid->isCellOccupied(pos.x, y))
    y --;
  bool result = (pos.y-1 == y);
  pos.y = y + 1;
  return result;
}

bool GridBit::drop(int distance)
{
  // TODO: Check all grid cells between the current position and required position before moving
  pos.y -= distance;
  return true;
}

/* ********** *
 * Grid class *
 * ********** */
Grid::Grid(int width, int height)
{
  this->width = width;
  this->height = height;
}

int Grid::getWidth() const
{
  return width;
}

int Grid::getHeight() const
{
  return height;
}

bool Grid::isCellOccupied(int x, int y) const
{
  Point p(x, y);
  for (list<GridBit>::const_iterator cur = gridBits.begin();
       cur != gridBits.end();
       cur++)
    if (p == cur->pos)
      return true;
  return false;
}

void Grid::placeBit(const ABit& bit, Colour c, int blockX, int blockY)
{
  gridBits.push_back(GridBit(bit, c, blockX, blockY, this));
}

bool Grid::timerTick(float dTime)
{
  // TODO: Check for full rows here and trigger animations
  bool result = false;
  for (list<GridBit>::iterator cur = gridBits.begin();
                               cur != gridBits.end();
                               cur++)
  {
    result |= cur->timerTick(dTime);
  }
  
  return result;
}

void Grid::render() const
{
  // TODO: Draw a grid somewhere around here
  for (list<GridBit>::const_iterator cur = gridBits.begin();
                                     cur != gridBits.end();
                                     cur++)
    cur->render();
}

