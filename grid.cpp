#include "common.h"

#define ACCELERATION 30.0f

#define VANISH_TIME 1.0f
#define NUM_SPINS_DURING_VANISH 3

/* ************* *
 * GridBit class *
 * ************* */
GridBit::GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, Grid *grid)
                                 : ABit(bit.pos.x + offsetX, bit.pos.y + offsetY)
{
  this->c = c;
  this->grid = grid;
  offsetY = speedY = 0.0f;
  moving = vanishing = false;
  vanishTime = 0.0f;
}

GridBit::GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, float vertOffset, float vertSpeed, Grid *grid)
                                 : ABit(bit.pos.x + offsetX, bit.pos.y + offsetY)
{
  this->c = c;
  this->grid = grid;
  this->offsetY = vertOffset;
  this->speedY = vertSpeed;
  moving = true;
  vanishing = false;
  vanishTime = 0.0f;
}

bool GridBit::timerTick(float dTime)
{
  if (!vanishing)
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
  else // if (vanishing)
  {
    vanishTime += dTime/VANISH_TIME;
    if (vanishTime >= 1.0f)
    {
      moving = false;
      return false;
    }
    
    return true;
  }
}

void GridBit::render() const
{
  if (!vanishing)
  {
    c.applyMaterial();
    comDrawTexturedCube(pos.x, pos.y + offsetY, 0.5f, 0);
  }
  else
  {
    float alpha = 1.0f - vanishTime;
    alpha *= alpha;
    c.applyMaterialAlpha(alpha);
    comDrawCube(pos.x, pos.y + offsetY, 0.5f*(1.0f - vanishTime), 360*vanishTime*NUM_SPINS_DURING_VANISH);
  }
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

bool GridBit::isMoving() const
{
  return moving;
}

void GridBit::triggerVanishing()
{
  vanishing = true;
}

bool GridBit::isVanishing() const
{
  return vanishing;
}

bool GridBit::operator==(const GridBit& bit) const
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
  bool result = false, full;
  
  { // Animate the vanishing bits
    list<GridBit> vanished;
    
    for (list<GridBit>::iterator cur=vanishingBits.begin(); cur != vanishingBits.end(); cur++)
    {
      if (cur->timerTick(dTime))
        result = true;
      else
        vanished.push_back(*cur);
    }
    
    if (!vanished.empty())
      for (list<GridBit>::const_iterator cur=vanished.begin(); cur != vanished.end(); cur++)
        vanishingBits.remove(*cur);
  }
  
  if (result)
    return true;
  
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
  
  // Check for full rows
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
    result = true;
    
    // Row is full, remove row and move the above blocks down
    GridBit* bit;
    for (int x=0; x < width; x++)
    {
      bit = getGridBit(x, y);
      if (bit != NULL)
      {
        vanishingBits.push_back(*bit);
        vanishingBits.back().triggerVanishing();
        gridBits[x].remove(*bit);
      }
      
      // move the blocks above down
      for (int y2=y+1; y2 < height; y2++)
      {
        bit = getGridBit(x, y2);
        if (bit != NULL)
          bit->drop(1);
      }
    }
    
    // Make sure we don't miss the new current row
    y--;
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
  for (list<GridBit>::const_iterator cur = vanishingBits.begin();
                                     cur != vanishingBits.end();
                                     cur++)
    cur->render();
}

