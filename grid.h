#define GRID_WIDTH 10
#define GRID_HEIGHT 20

class ABit;
class Grid;

class GridBit : public ABit {
  private:
    Bezier offsetX, offsetY;
    Grid* grid;
  
  public:
    GridBit(const ABit& bit, int offsetX, int offsetY, Grid *grid);
    
    void timerTick(float dTime);
    void render() const;
};

class Grid {
  private:
    int width, height;
  
  public:
    Grid(int width, int height);
    
    int getWidth() const;
    int getHeight() const;
    bool isCellOccupied(int x, int y) const;
    void placeBit(const ABit& bit, int x, int y);
    void timerTick(float dTime);
    void render() const;
};
