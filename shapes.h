// Constants:
#define SHAPE_TYPE_NOT_READY 5000
#define SHAPE_TYPE_NORMAL 5001
#define SHAPE_TYPE_SOFT 5002
#define SHAPE_TYPE_BRITTLE 5004

class Point {
  public:
    int x, y;
    
    Point();
    Point(const Point& p);
    Point(int x, int y);
};

struct ABit {
  Point pos;
  // TODO: Add something about which power-up this has
  ABit(int x, int y)
  {
    pos = Point(x, y);
  }
};

class Shape {
  private:
    vector<ABit> the_bits;
    int type, size;
    Colour c;
    Point pos;
    // Interpolation stuff
    float offset_x, offset_y;
    float distGone_x, distGone_y;
  
  public:
    Shape();
    Shape(FILE* in);
    
    // Returns true if this shape collides with anything in the grid
    bool collides() const;
    // Try to move this shape. Returns true if we could move it, false otherwise
    bool move(int dx, int dy);
    // Try to rotate this shape to the right. Returns true if we could turn it, false otherwise
    bool rotateRight();
    // Try rotating this shape to the left. Returns true if it worked, false otherwise
    bool rotateLeft();
    // Perform one step in the animation. Returns true if this shape moved
    bool animate(float dTime, float curTime);
    // Draw the shape
    void draw() const;
};

void shInit();

