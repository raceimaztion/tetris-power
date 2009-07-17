#define GRID_WIDTH 10
#define GRID_HEIGHT 20

class ABit;
class Grid;

class GridBit : public ABit {
  private:
    float offsetX, speedX;
    Grid* grid;
    Colour c;
  
  public:
    GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, Grid *grid);
    
    bool timerTick(float dTime);
    void render() const;
    bool drop();
    bool drop(int distance);
};

class Grid {
  private:
    int width, height;
    list<GridBit>* gridBits;
  
  public:
    Grid(int width, int height);
    
    int getWidth() const;
    int getHeight() const;
    bool isCellOccupied(int x, int y) const;
    void placeBit(const ABit& bit, Colour c, int x, int y);
    bool timerTick(float dTime);
    void render() const;
};
