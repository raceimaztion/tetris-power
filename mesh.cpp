#define MESH_MODULE
#include "common.h"

/* ********************* *
 * Polygon class methods *
 * ********************* */
Polygon::Polygon(vector<int> &vertices, vector<int> &textures)
{
  this->vertices = vertices;
  this->textures = textures;
  smooth = false;
}

Polygon::Polygon(vector<int> &vertices, vector<int> &textures, vector<int> &normals)
{
  this->vertices = vertices;
  this->textures = textures;
  this->normals = normals;
  smooth = true;
}

Polygon::Polygon(vector<int> &vertices, vector<int> &textures, vector<int> &normals, bool smooth)
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
  loadWavefrontObjectFile(&result, in);
  return result;
}

Mesh Mesh::loadWavefrontObjectFile(string filename)
{
  FILE* in = fopen(filename.c_str(), "r");
  if (in == NULL)
    fprintf(stderr, "Warning: Failed to open wavefront object file '%s'.\n", filename.c_str());
  
  Mesh result;
  
  loadWavefrontObjectFile(&result, in);
  
  return result;
}

void Mesh::loadWavefrontObjectFile(Mesh *mesh, FILE *in)
{
  if (mesh == NULL || in == NULL)
    return;
  
  bool smooth = false;
  int numSmooth = 0;
  
  string line = comReadLine(in);
  while (!feof(in))
  {
    if (line.find("#") == 0 ||
        line.find("//") == 0)
    {
      line = comReadLine(in);
      continue;
    }
    else if (line.find("v ") == 0)
    {
      float x, y, z;
      sscanf(line.c_str(), "%*s %f %f %f", &x, &y, &z);
      
      mesh->addVertex(x, y, z);
#ifdef DEBUG
//      printf("v"); fflush(stdout);
#endif
    }
    else if (line.find("vn ") == 0)
    {
      float x, y, z;
      sscanf(line.c_str(), "%*s %f %f %f", &x, &y, &z);
      
      mesh->addNormal(x, y, z);
#ifdef DEBUG
//      printf("n"); fflush(stdout);
#endif
    }
    else if (line.find("vt ") == 0)
    {
      float u, v=0.0f, w=0.0f;
      sscanf(line.c_str(), "%*s %f %f %f", &u, &v, &w);
      
      mesh->addTexture(u, v, w);
#ifdef DEBUG
//      printf("t"); fflush(stdout);
#endif
    }
    else if (line.find("s ") == 0)
    {
      if (line.find("off") != string::npos ||
          line.find("0") != string::npos)
        smooth = false;
      else
        smooth = true;
    }
    else if (line.find("f ") == 0)
    {
      vector<int> vertices, textures, normals;
      
      vector<string> split = comSplitSpaces(line.substr(2));
      for (unsigned int i=0; i < split.size(); i++)
      {
        // Does this polygon have normals and/or textures specified?
        if (split[i].find("/") != string::npos)
        {
          vector<string> spliced = comSplitString(split[i], "/");
          while (spliced.size() < 3) spliced.push_back("");
          
          // Grab the vertex number
          vertices.push_back(atoi(spliced.at(0).c_str()) - 1);
          // Do we have a texture coordinate for this vertex?
          if (spliced.at(1).size() > 0)
            textures.push_back(atoi(spliced.at(1).c_str()) - 1);
          else
            textures.push_back(-1);
          // Do we have a normal for this vertex?
          if (spliced.at(2).size() > 0)
            normals.push_back(atoi(spliced.at(2).c_str()) - 1);
          else
            normals.push_back(-1);
        }
        else
        {
          vertices.push_back(atoi(split.at(i).c_str()) - 1);
          normals.push_back(-1);
        }
      }
      
      mesh->addPolygon(Polygon(vertices, textures, normals, smooth));
      if (smooth) numSmooth ++;
    }
    
    line = comReadLine(in);
  }
  
  printf("Information: Mesh::loadWavefrontObjectFile(): Finished loading mesh data, parsed %d vertices, %d texture coordinates, %d normals, %d polygons, and %d smooth faces.\n",
         mesh->vertices.size(), mesh->textures.size(), mesh->normals.size(), mesh->polygons.size(), numSmooth);
}

void Mesh::loadWavefrontObjectFile(Mesh* mesh, string filename)
{
  FILE* in = fopen(filename.c_str(), "r");
  if (in == NULL)
    fprintf(stderr, "Error: Failed to open wavefront object file '%s'.\n", filename.c_str());
  else
    loadWavefrontObjectFile(mesh, in);
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

void Mesh::addPolygon(vector<int>& vertices, vector<int>& textures)
{
  addPolygon(Polygon(vertices, textures));
}

void Mesh::addPolygon(vector<int>& vertices,
                      vector<int>& textures,
                      vector<int>& normals)
{
  addPolygon(Polygon(vertices, textures, normals));
}

// Post-load methods
void Mesh::render(bool use_textures) const
{
  bool smooth;
  
  for (unsigned int i=0; i < polygons.size(); i++)
  {
    switch(polygons.at(i).vertices.size())
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
        //continue;
        printf("  Warning: Mesh::render(): Polygons with %d vertices not supported.\n", polygons.at(i).vertices.size());
        continue;
        //goto again;
    }
    
    smooth = polygons.at(i).smooth;
    if (!smooth && polygons.at(i).normals.at(0) >= 0)
    {
      normals.at(polygons.at(i).normals.at(0)).applyNormal();
    }
    
    for (unsigned int j=0; j < polygons.at(i).vertices.size(); j++)
    {
      if (smooth && polygons.at(i).normals.at(j) >= 0)
      {
        normals.at(polygons.at(i).normals.at(j)).applyNormal();
      }
      
      if (use_textures && polygons.at(i).textures.at(j) >= 0)
        textures.at(polygons.at(i).textures.at(j)).applyTexCoords();
      
      vertices.at(polygons.at(i).vertices.at(j)).applyVertex();
    }
    
    glEnd();
    
//again: ;
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

void Mesh::compileList(bool use_textures)
{
  if (compiled)
    return;
  
  listIndex = glGenLists(1);
  compiled = true;
  
  glNewList(listIndex, GL_COMPILE);
  render(use_textures);
  glEndList();
}

void Mesh::renderList() const
{
  if (!compiled)
    render();
  else
    glCallList(listIndex);
}
