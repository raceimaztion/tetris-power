#include "common.h"

#define ACCELERATION 5.0f

/* ************* *
 * GridBit class *
 * ************* */
GridBit::GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, Grid *grid)
                                 : ABit(bit.pos.x + offsetX, bit.pos.y + offsetY)
{
  this->c = c;
  this->grid = grid;
  offsetY = speedY = 0.0f;
  moving = false;
}

GridBit::GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, float vertOffset, float vertSpeed, Grid *grid)
                                 : ABit(bit.pos.x + offsetX, bit.pos.y + offsetY)
{
  this->c = c;
  this->grid = grid;
  this->offsetY = vertOffset;
  this->speedY = vertSpeed;
  moving = true;
}

bool GridBit::timerTick(float dTime)
{
  if (moving)
  {
    speedY += ACCELERATION*dTime;
    offsetY -= speedY*dTime;
    if (offsetY < 0.0f)
    {
      offsetY = speedY = 0.0f;
      moving = false;
    }
    
    return true;
  }
  else
    return false;
}

void GridBit::render() const
{
  c.applyMaterial();
  comDrawCube(pos.x, pos.y + offsetY, 0.5f, 0);
}

bool GridBit::drop()
{
  int curY = pos.y;
  int y = pos.y - 1;
  while (!grid->isCellOccupied(pos.x, y))
    y --;
  bool result = (pos.y-1 == y);
  pos.y = y + 1;
  
  if (result)
    offsetY -= pos.y - curY;
  
  return result;
}

bool GridBit::drop(int distance)
{
  // TODO: Check all grid cells between the current position and required position before moving
  pos.y -= distance;
  offsetY += distance;
  moving = true;
  
  return true;
}

bool GridBit::operator==(const GridBit& bit)
{
  return (pos.x == bit.pos.x) && (pos.y == bit.pos.y);
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

GridBit* Grid::getGridBit(int x, int y)
{
  if (x < 0 || y < 0 || x >= width || y >= height)
    return NULL;
  
  Point p(x, y);
  for (list<GridBit>::iterator cur = gridBits[x].begin();
                               cur != gridBits[x].end();
                               cur++)
  {
    if (cur->pos == p)
      return &(*cur);
  }
  
  return NULL;
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

void Grid::placeBit(const ABit& bit, Colour c, int blockX, int blockY, float offsetY, float speedY)
{
  GridBit newBit(bit, c, blockX, blockY, offsetY, speedY, this);
  if (newBit.pos.x < 0 || newBit.pos.x >= width)
    return;
  
  gridBits[newBit.pos.x].push_back(newBit);
}

bool Grid::timerTick(float dTime)
{
  // TODO: Check for full rows here and trigger animations
  bool result = false, full;
  
  // Check for full rows
  // Trigger animations
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
  
  if (result)
    return true;
  
  for (int y=0; y < height; y++)
  {
    full = true;
    
    for (int x=0; x < width; x++)
      if (!isCellOccupied(x, y))
      {
        full = false;
        break;
      }
    
    if (!full) continue;
    
    // Row is full, remove row and move the above blocks down
    GridBit* bit;
    for (int x=0; x < width; x++)
    {
      bit = getGridBit(x, y);
      if (bit != NULL)
        gridBits[x].remove(*bit);
      
      // move the blocks above down
      for (int y2=y+1; y2 < height; y2++)
      {
        bit = getGridBit(x, y2);
        if (bit != NULL)
          bit->drop(1);
      }
    }
  }// end for y
  
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

