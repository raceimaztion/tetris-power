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
/*  if (offsetX < 0.0f)
  {
    speedX += ACCELERATION*dTime;
    offsetX += speedX*dTime;
    if (offsetX > 0.0f)
      offsetX = speedX = 0.0f;
    
    return true;
  }
  else*/
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
  
  gridBits = new list<GridBit>[width];
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
  if (x < 0 || y < 0 || x >= width || y >= height)
    return true;
  
  Point p(x, y);
  for (list<GridBit>::const_iterator cur = gridBits[x].begin();
       cur != gridBits[x].end();
       cur++)
    if (p == cur->pos)
      return true;
  return false;
}

void Grid::placeBit(const ABit& bit, Colour c, int blockX, int blockY)
{
  GridBit newBit(bit, c, blockX, blockY, this);
  if (newBit.pos.x < 0 || newBit.pos.x >= width)
    return;
  
  gridBits[newBit.pos.x].push_back(newBit);
}

bool Grid::timerTick(float dTime)
{
  // TODO: Check for full rows here and trigger animations
  bool result = false;
  for (int x=0; x < width; x++)
  {
    for (list<GridBit>::iterator cur = gridBits[x].begin();
                                 cur != gridBits[x].end();
                                 cur++)
    {
      if (cur->timerTick(dTime))
        result = true;
    }
  }
  
  return result;
}

void Grid::render() const
{
  // TODO: Draw a grid somewhere around here
  for (int x=0; x < width; x++)
  {
    for (list<GridBit>::const_iterator cur = gridBits[x].begin();
                                       cur != gridBits[x].end();
                                       cur++)
      cur->render();
  }
}

