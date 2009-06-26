#define MESH_MODULE
#include "common.h"

/* ********************* *
 * Polygon class methods *
 * ********************* */
Polygon::Polygon(vector<int> vertices, vector<int> textures)
{
  this->vertices = vertices;
  this->textures = textures;
  smooth = false;
}

Polygon::Polygon(vector<int> vertices, vector<int> textures, vector<int> normals)
{
  this->vertices = vertices;
  this->textures = textures;
  this->normals = normals;
  smooth = true;
}

Polygon::Polygon(vector<int> vertices, vector<int> textures, vector<int> normals, bool smooth)
{
  this->vertices = vertices;
  this->textures = textures;
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

Mesh::Mesh(const Mesh& m)
{
  vertices = m.vertices;
  textures = m.textures;
  normals = m.normals;
  polygons = m.polygons;
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
    else if (line.find_first_of("vt ") == 0)
    {
      float u, v, w;
      sscanf(line.c_str(), "%*s %f %f %f", &u, &v, &w);
      
      result.addTexture(u, v, w);
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
      vector<int> vertices, textures, normals;
      
      vector<string> split = comSplitSpaces(line.substr(2));
      for (unsigned int i=0; i < split.size(); i++)
      {
        if (split[i].find_first_of("/") != string::npos)
        {
          vector<string> spliced = comSplitString(split[i], "/");
          vertices.push_back(atoi(spliced[0].c_str()));
          if (spliced[1].size() > 0)
            textures.push_back(atoi(spliced[1].c_str()));
          else
            textures.push_back(-1);
          normals.push_back(atoi(spliced[2].c_str()));
        }
        else
        {
          vertices.push_back(atoi(split[i].c_str()));
          normals.push_back(-1);
        }
      }
      
      result.addPolygon(Polygon(vertices, textures, normals, smooth));
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

void Mesh::addTexture(const Position& t)
{
  textures.push_back(t);
}

void Mesh::addTexture(float u, float v, float w)
{
  addTexture(Position(u, v, w));
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

void Mesh::addPolygon(const vector<int>& vertices, const vector<int>& textures)
{
  addPolygon(Polygon(vertices, textures));
}

void Mesh::addPolygon(const vector<int>& vertices,
                      const vector<int>& textures,
                      const vector<int>& normals)
{
  addPolygon(Polygon(vertices, textures, normals));
}

// Post-load methods
void Mesh::render(bool use_textures)
{
  bool smooth;
  
  for (unsigned int i=0; i < polygons.size(); i++)
  {
    switch(polygons[i].vertices.size())
    {
      case 1:
        glBegin(GL_POINTS);
        break;
      case 2:
        glBegin(GL_LINES);
        break;
      case 3:
        glBegin(GL_TRIANGLES);
        break;
      case 4:
        glBegin(GL_QUADS);
        break;
      default:
        goto again;
    }
    
    smooth = polygons[i].smooth;
    if (!smooth)
    {
      if (polygons[i].normals[0] >= 0)
        normals[polygons[i].normals[0]].applyNormal();
    }
    
    for (unsigned int j=0; j < polygons[i].vertices.size(); j++)
    {
      if (smooth && polygons[i].normals[j] >= 0)
        normals[polygons[i].normals[j]].applyNormal();
      
      if (use_textures && polygons[i].textures[j] >= 0)
        textures[polygons[i].textures[j]].applyTexCoords();
      
      vertices[polygons[i].vertices[j]].applyVertex();
    }
    
    glEnd();
    
again: ;
  } // end for each polygon
} // end Mesh::render()

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

