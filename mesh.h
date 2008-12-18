/* ************************************ *
 * Mesh module                          *
 * Handles loading and rendering meshes *
 * ************************************ */

/* ************************************************ *
 * A Polygon object doesn't know anything about the *
 *   actual vertices, it just has index information *
 * ************************************************ */
class Polygon {
  public:
    vector<int> vertices, normals;
    bool smooth;
    
    Polygon(vector<int> vertices);
    Polygon(vector<int> vertices, vector<int> normals);
};

/* ************************************************************* *
 * A Mesh object only contains geometry information, no textures *
 * ************************************************************* */
class Mesh {
  private:
    vector<Position> vertices, normals;
    vector<Polygon> polygons;
  
  public:
    // Constructors/destructors
    Mesh();
    ~Mesh();
    
    // Static methods used for loading a file
    static Mesh loadWavefrontObjectFile(FILE *in);
    static Mesh loadWavefrontObjectFile(string filename);
    
    // Methods used during load-time
    void addVertex(const Position& p);
    void addVertex(float x, float y, float z);
    void addNormal(const Position& p);
    void addNormal(float x, float y, float z);
    void addPolygon(const Polygon& p);
    void addPolygon(const vector<int>& vertices); // flat-shaded polygon
    void addPolygon(const vector<int>& vertices,
                    const vector<int>& normals); // smooth-shaded polygon
    
    // Post-load methods
    void render();
    void translate(const Position& p);
    void translate(float x, float y, float z);
};

// "Interface" functions to the static methods in the Mesh class
Mesh loadWavefrontObjectFile(string filename);

