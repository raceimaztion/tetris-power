#define MESH_MODULE
#include "common.h"

/* ********************* *
 * Polygon class methods *
 * ********************* */
Polygon::Polygon(vector<int> vertices)
{
  this->vertices = vertices;
  smooth = false;
}

Polygon::Polygon(vector<int> vertices, vector<int> normals)
{
  this->vertices = vertices;
  this->normals = normals;
  smooth = true;
}

Polygon::Polygon(vector<int> vertices, vector<int> normals, bool smooth)
{
  this->vertices = vertices;
  this->normals = normals;
  this->smooth = smooth;
}

/* ****************** *
 * Mesh class methods *
 * ****************** */
// Constructors/destructors
Mesh::Mesh()
{
  // Really don't need to do anything here
}

Mesh::~Mesh()
{
  // Really don't need to do anything here
}

// Static methods
Mesh Mesh::loadWavefrontObjectFile(FILE *in)
{
  Mesh result;
  bool smooth = false;
  
  string line = comReadLine(in);
  while (!feof(in))
  {
    if (line.find_first_of("#") == 0 ||
        line.find_first_of("//") == 0)
    {
      line = comReadLine(in);
      continue;
    }
    else if (line.find_first_of("v ") == 0)
    {
      float x, y, z;
      sscanf(line.c_str(), "%*s %f %f %f", &x, &y, &z);
      
      result.addVertex(x, y, z);
    }
    else if (line.find_first_of("vn ") == 0)
    {
      float x, y, z;
      sscanf(line.c_str(), "%*s %f %f %f", &x, &y, &z);
      
      result.addNormal(x, y, z);
    }
    else if (line.find_first_of("s ") == 0)
    {
      if (line.find_first_of("off") != string::npos ||
          line.find_first_of("0") != string::npos)
        smooth = false;
      else
        smooth = true;
    }
    else if (line.find_first_of("f ") == 0)
    {
      vector<int> vertices, normals;
      
      // TODO: add parsing code here
    }
    
    line = comReadLine(in);
  }
  
  return result;
}

Mesh Mesh::loadWavefrontObjectFile(string filename)
{
  FILE* in = fopen(filename.c_str(), "r");
  if (in == NULL)
  {
    fprintf(stderr, "Failed to open wavefront object file '%s'.\n", filename.c_str());
    return Mesh();
  }
  
  return loadWavefrontObjectFile(in);
}

// Methods used during load-time
void Mesh::addVertex(const Position& v)
{
  vertices.push_back(v);
}

void Mesh::addVertex(float x, float y, float z)
{
  addVertex(Position(x, y, z));
}

void Mesh::addNormal(const Position& n)
{
  normals.push_back(n);
}

void Mesh::addNormal(float x, float y, float z)
{
  addNormal(Position(x, y, z));
}

void Mesh::addPolygon(const Polygon& p)
{
  polygons.push_back(p);
}

void Mesh::addPolygon(const vector<int>& vertices)
{
  addPolygon(Polygon(vertices));
}

void Mesh::addPolygon(const vector<int>& vertices,
                      const vector<int>& normals)
{
  addPolygon(Polygon(vertices, normals));
}

// Post-load methods
void Mesh::render()
{
  // TODO: finish this
}

void Mesh::translate(const Position& p)
{
  for(unsigned int i=0; i < vertices.size(); i++)
  {
    vertices[i] += p;
  }
}

void Mesh::translate(float x, float y, float z)
{
  translate(Position(x, y, z));
}

