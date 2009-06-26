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
    vector<int> vertices, textures, normals;
    bool smooth;
    
    Polygon(vector<int> vertices, vector<int> textures);
    Polygon(vector<int> vertices, vector<int> textures, vector<int> normals);
    Polygon(vector<int> vertices, vector<int> textures, vector<int> normals, bool smooth);
};

/* ************************************************************* *
 * A Mesh object only contains geometry information, no textures *
 * ************************************************************* */
class Mesh {
  private:
    vector<Position> vertices, textures, normals;
    vector<Polygon> polygons;
  
  public:
    // Constructors/destructors
    Mesh();
    Mesh(const Mesh& m);
    ~Mesh();
    
    // Static methods used for loading a file
    static Mesh loadWavefrontObjectFile(FILE *in);
    static Mesh loadWavefrontObjectFile(string filename);
    
    // Methods used during load-time
    void addVertex(const Position& p);
    void addVertex(float x, float y, float z);
    void addTexture(const Position& t);
    void addTexture(float u, float v, float w);
    void addNormal(const Position& p);
    void addNormal(float x, float y, float z);
    void addPolygon(const Polygon& p);
    void addPolygon(const vector<int>& vertices,
                    const vector<int>& textures); // flat-shaded polygon
    void addPolygon(const vector<int>& vertices,
                    const vector<int>& textures,
                    const vector<int>& normals); // smooth-shaded polygon
    
    // Post-load methods
    void render(bool use_textures=false);
    void translate(const Position& p);
    void translate(float x, float y, float z);
};

// "Interface" functions to the static methods in the Mesh class
Mesh loadWavefrontObjectFile(string filename);

