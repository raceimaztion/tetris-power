// Constants:
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

class Shape {
  private:
    vector<Point> the_bits;
    int type;
    Colour c;
  
  public:
    Shape();
    Shape(FILE* in);
    
    bool collides();
    bool move(int dx, int dy);
    bool animate(float dTime, float curTime);
    void draw();
};

void shInit();

