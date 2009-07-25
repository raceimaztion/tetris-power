#define GRID_WIDTH 10
#define GRID_HEIGHT 20

class ABit;
class Grid;

class GridBit : public ABit {
  private:
    float offsetY, speedY;
    Grid* grid;
    Colour c;
    bool moving;
  
  public:
    GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, Grid *grid);
    GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, float vertOffset, float vertSpeed, Grid *grid);
    
    bool timerTick(float dTime);
    void render() const;
    bool drop();
    bool drop(int distance);
    
    bool operator==(const GridBit& bit);
};

class Grid {
  private:
    int width, height;
    list<GridBit>* gridBits;
    // Useful methods
    GridBit* getGridBit(int x, int y);
  
  public:
    Grid(int width, int height);
    
    int getWidth() const;
    int getHeight() const;
    bool isCellOccupied(int x, int y) const;
    void placeBit(const ABit& bit, Colour c, int x, int y);
    void placeBit(const ABit& bit, Colour c, int x, int y, float offsetY, float speedY);
    bool timerTick(float dTime);
    void render() const;
};

