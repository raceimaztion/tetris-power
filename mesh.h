/* ************************************ *
 * Mesh module                          *
 * Handles loading and rendering meshes *
 * ************************************ */

/* ************************************************************* *
 * A Mesh object only contains geometry information, no textures *
 * ************************************************************* */
class Mesh {
  private:
    vector<Position> vertices, normals;
    vector<vector<int> > polygons, polygon_normals;
    vector<bool> polygon_smooth;
  
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
    void addPolygon(const vector<int>& vertices); // flat-shaded polygon
    void addPolygon(const vector<int>& vertices,
                    const vector<int>& normals); // smooth-shaded polygon
    
    // Post-load methods
    void render();
    void translate(float x, float y, float z);
};

