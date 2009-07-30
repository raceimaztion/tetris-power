#define GRID_WIDTH 10
#define GRID_HEIGHT 20

class ABit;
class Grid;

class GridBit : public ABit {
  private:
    float offsetY, speedY;
    Grid* grid;
    Colour c;
    bool moving, vanishing;
    float vanishTime;
  
  public:
    GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, Grid *grid);
    GridBit(const ABit& bit, Colour c, int offsetX, int offsetY, float vertOffset, float vertSpeed, Grid *grid);
    
    bool timerTick(float dTime);
    void render() const;
    bool drop();
    bool drop(int distance);
    bool isMoving() const;
    void triggerVanishing();
    bool isVanishing() const;
    
    bool operator==(const GridBit& bit) const;
};

class GridListener {
  public:
    virtual void rowRemoved(int row);
    virtual void gridEmpty();
};

class Grid {
  private:
    int width, height;
    list<GridBit>* gridBits;
    list<GridBit> vanishingBits;
    list<GridListener*> listeners;
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
    
    void addListener(GridListener* listener);
    void removeListener(GridListener* listener);
};

